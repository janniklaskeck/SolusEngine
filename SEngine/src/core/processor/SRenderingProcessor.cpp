#include "core/processor/SRenderingProcessor.hpp"

#include "assets/SAssetManager.hpp"
#include "core/processor/SProcessorManager.hpp"
#include "core/SEngine.hpp"
#include "core/SWorld.hpp"
#include "entity/component/SCameraComponent.hpp"
#include "entity/component/SMeshComponent.hpp"
#include "entity/component/STransformComponent.hpp"
#include "render/editor/SEditorInterface.hpp"
#include "render/SRenderGraph.hpp"
#include "render/SWindow.hpp"
#include "render/text/STextRenderer.hpp"
#include "utils/font/font_manager.h"
#include "utils/font/text_buffer_manager.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bx/string.h>
#include <SDL3/SDL.h>

#include <cstdio>

namespace Solus
{
	struct FSQVertexData
	{
		float m_x, m_y, m_z;
		float m_u, m_v;

		static bgfx::VertexLayout VertexLayout;
	};

	bgfx::VertexLayout FSQVertexData::VertexLayout;

	SRenderingProcessor::SRenderingProcessor()
	{
		SWindow* Window = gEngine->GetWindow();

		CurrentWindowWidth = Window->GetWindowWidth();
		CurrentWindowHeight = Window->GetWindowHeight();

		bgfx::Init BGFX_Init;
		BGFX_Init.platformData.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(Window->GetSDLWindow()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
		BGFX_Init.platformData.ndt = nullptr;
		BGFX_Init.type = bgfx::RendererType::Vulkan;
		BGFX_Init.resolution.width = CurrentWindowWidth;
		BGFX_Init.resolution.height = CurrentWindowHeight;
		BGFX_Init.resolution.reset = BGFX_RESET_VSYNC;

		if (!bgfx::init(BGFX_Init))
		{
			fprintf(stderr, "bgfx init failed\n");
			return;
		}

		bgfx::setDebug(BGFX_DEBUG_NONE);

		FSQVertexData::VertexLayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();

		TextRenderer = std::make_unique<STextRenderer>(this);

		UniformManager.CreateUniform("u_color", bgfx::UniformType::Vec4);
		UniformManager.CreateUniform("s_shadowMap", bgfx::UniformType::Sampler);
		UniformManager.CreateUniform("u_lightPos", bgfx::UniformType::Vec4);
		UniformManager.CreateUniform("u_lightMtx", bgfx::UniformType::Mat4);
		UniformManager.CreateUniform("u_depthScaleOffset", bgfx::UniformType::Vec4);

		UniformManager.CreateUniform("s_texColor", bgfx::UniformType::Sampler);

		ShadowMapProgramAsset = SAssetManager::Get().GetAsset<SShaderProgramAsset>();
		ShadowMapProgramAsset->SetPaths("shaders/vs_shadow.sc", "shaders/fs_shadow.sc");
		ShadowMapProgramHandle = ShaderProgramCache.GetProgramHandle(ShadowMapProgramAsset);

		FullscreenQuadProgramAsset = SAssetManager::Get().GetAsset<SShaderProgramAsset>();
		FullscreenQuadProgramAsset->SetPaths("shaders/vs_quad.sc", "shaders/fs_quad.sc");
		FullscreenQuadProgramHandle = ShaderProgramCache.GetProgramHandle(FullscreenQuadProgramAsset);

		// Get renderer capabilities info.
		const bgfx::Caps* caps = bgfx::getCaps();
		float depthScaleOffset[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
		if (caps->homogeneousDepth)
		{
			depthScaleOffset[0] = 0.5f;
			depthScaleOffset[1] = 0.5f;
		}

		UniformManager.SetUniform("u_depthScaleOffset", depthScaleOffset);

		ShadowCamera.Data.Type = SCameraType::Orthographic;
		ShadowCamera.Data.OrthographicArea = { -300.f, 300.f, -300.f, 300.f };
		ShadowCamera.Data.NearPlane = -1000.f;
		ShadowCamera.Data.FarPlane = 1000.f;

		UICamera.Data.Type = SCameraType::Orthographic;
		UICamera.Data.OrthographicArea = { 0.f, CurrentWindowWidth, CurrentWindowHeight, 0.f };
		UICamera.Data.NearPlane = 0.f;
		UICamera.Data.FarPlane = 100.f;

		EditorCamera.Data.NearPlane = 0.1f;
		EditorCamera.Data.FarPlane = 1000.f;
		EditorCamera.Data.FOV_Y = 90.f;

		WorldRenderView.ViewId = ViewID_World;
		EditorRenderView.ViewId = ViewID_Editor;
		ShadowRenderView.ViewId = ViewID_Shadow;
		UIRenderView.ViewId = ViewID_UI;

		InitSceneView(EditorRenderView, 800, 400);
		InitSceneView(WorldRenderView, CurrentWindowWidth, CurrentWindowHeight);
		InitSceneView(ShadowRenderView, ShadowMapSize, ShadowMapSize, true);
		InitSceneView(UIRenderView, CurrentWindowWidth, CurrentWindowHeight);

		SEditorInterface::Init();

		Reset(false);
	}

	SRenderingProcessor::~SRenderingProcessor()
	{
		SEditorInterface::Destroy();

		UniformManager.Destroy();
		TextRenderer.reset();
		bgfx::shutdown();
	}

	/*
	include/RenderGraph.h — the public API

	TextureHandle / BufferHandle — typed, 16-bit IDs; invalid sentinel is UINT16_MAX
	TextureDesc / BufferDesc — resource descriptors with relative/absolute sizing
	PassBuilder — used inside the setup lambda: createTexture, readTexture, writeTexture, setView()
	RenderGraph — the main class: addPass, importTexture, compile, execute, reset

	src/RenderGraph.cpp — implementation

	Culling: iterates passes to a fixed point, marking any pass whose outputs have zero refcount as culled (dead code elimination equivalent)
	Physical resource creation: resolves relative sizes against the backbuffer, calls bgfx::createTexture2D + wraps RT textures in bgfx::FrameBufferHandle
	reset() destroys transient resources but keeps imported ones

	src/main.cpp — wired example
	Three passes declared with lambdas:

	ShadowPass → 2048×2048 D24S8 shadow map
	GeometryPass → reads shadow map, writes RGBA16F HDR + depth
	TonemapPass → reads HDR, writes to backbuffer via a fullscreen quad

	CMakeLists.txt — drop bgfx/bx/bimg alongside and adjust BGFX_ROOT.

	Key extension points to fill in:

	submitFullscreenQuad → cache a screen-space quad VB
	loadProgram(vs, fs) → read compiled .bin shaders from disk
	Scene iteration in geometry/shadow execute lambdas
	Buffer physical allocation in createPhysicalResources (stub left for your vertex/index buffer strategy)

	//
	//  main.cpp  —  render graph usage example
	//
	//  Demonstrates three passes wired together:
	//    1. ShadowPass    → renders depth into a shadow map
	//    2. GeometryPass  → opaque geometry, reads shadow map, writes HDR colour + depth
	//    3. TonemapPass   → post-process: reads HDR colour, writes to backbuffer
	//
	//  bgfx init / window creation is left to your platform layer.
	//
	*/

	void SRenderingProcessor::Tick(const float DeltaTime)
	{
		SRenderView& MainRenderView = WorldRenderView;
		if (gEngine->IsInEditor())
		{
			MainRenderView = EditorRenderView;

			SEditorInterface::BeginInterface();

			SEditorInterface::RenderInterface(DeltaTime);
			SEditorInterface::RenderEditorScene(EditorRenderView);

			SEditorInterface::EndInterface();
		}

		bgfx::touch(ViewID_Default);
		bgfx::touch(244);
		bgfx::touch(MainRenderView.ViewId);
		bgfx::touch(ShadowRenderView.ViewId);
		bgfx::touch(UIRenderView.ViewId);

		bgfx::setViewFrameBuffer(MainRenderView.ViewId, MainRenderView.FrameBufferHandle);
		bgfx::setViewRect(MainRenderView.ViewId, 0, 0, MainRenderView.Width, MainRenderView.Height);
		bgfx::setViewClear(MainRenderView.ViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		bgfx::setViewFrameBuffer(ShadowRenderView.ViewId, ShadowRenderView.FrameBufferHandle);
		bgfx::setViewRect(ShadowRenderView.ViewId, 0, 0, ShadowRenderView.Width, ShadowRenderView.Height);
		bgfx::setViewClear(ShadowRenderView.ViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		bgfx::setViewFrameBuffer(UIRenderView.ViewId, UIRenderView.FrameBufferHandle);
		bgfx::setViewRect(UIRenderView.ViewId, 0, 0, UIRenderView.Width, UIRenderView.Height);
		bgfx::setViewClear(UIRenderView.ViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		RenderWorld(MainRenderView);

		//SetupCamera(UICamera, UIRenderView);
		RenderUI(UIRenderView);

		if (!gEngine->IsInEditor())
		{
			bgfx::setTexture(0, UniformManager.GetUniformHandle("s_texColor"), MainRenderView.ViewTexture);
			bgfx::setViewFrameBuffer(244, BGFX_INVALID_HANDLE);
			bgfx::setViewRect(244, 0, 0, CurrentWindowWidth, CurrentWindowHeight);
			SetFullscreenQuad();

			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			bgfx::submit(244, FullscreenQuadProgramHandle);
		}

		bgfx::frame();
	}

	int32 SRenderingProcessor::GetPriority() const
	{
		return ProcessorPriority::Rendering;
	}

/*	void SRenderer::RenderFrame()
	{
		SRenderGraph graph;

		// ── Declare transient resources ──────────────────────────────────────────

		STextureHandle shadowMap;
		STextureHandle hdrColor;
		STextureHandle sceneDepth;

		// ── Pass 1: Shadow map ───────────────────────────────────────────────────

		graph.AddPass("ShadowPass",
			[&](SPassBuilder& builder)
			{
				STextureDesc shadowDesc;
				shadowDesc.Name = "ShadowMap";
				shadowDesc.Scale = STextureScale::Absolute;
				shadowDesc.Width = 2048;
				shadowDesc.Height = 2048;
				shadowDesc.Format = bgfx::TextureFormat::D24S8;
				shadowDesc.Flags = BGFX_TEXTURE_RT;

				shadowMap = builder.CreateTexture(shadowDesc);
				builder.SetView(kViewShadow);
			},
			[&](const SRenderGraph& rg, bgfx::ViewId view)
			{
				bgfx::FrameBufferHandle fb = rg.GetFramebuffer(shadowMap);

				bgfx::setViewFrameBuffer(view, fb);
				bgfx::setViewRect(view, 0, 0, 2048, 2048);
				bgfx::setViewClear(view, BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);

				// TODO: set light-space view/proj matrices via bgfx::setViewTransform
				// TODO: iterate scene and submit shadow casters
				//   bgfx::setTransform(&modelMtx);
				//   bgfx::setVertexBuffer(0, mesh.vbh);
				//   bgfx::setIndexBuffer(mesh.ibh);
				//   bgfx::submit(view, g_shadowProgram);

				printf("[ShadowPass] submitted to view %u\n", (unsigned)view);
			}
		);

		// ── Pass 2: Geometry ─────────────────────────────────────────────────────

		graph.AddPass("GeometryPass",
			[&](SPassBuilder& builder)
			{
				builder.ReadTexture(shadowMap);   // declares read dependency

				STextureDesc colorDesc;
				colorDesc.Name = "HDRColor";
				colorDesc.Format = bgfx::TextureFormat::RGBA16F;
				colorDesc.Flags = BGFX_TEXTURE_RT;
				hdrColor = builder.CreateTexture(colorDesc);

				STextureDesc depthDesc;
				depthDesc.Name = "SceneDepth";
				depthDesc.Format = bgfx::TextureFormat::D24S8;
				depthDesc.Flags = BGFX_TEXTURE_RT;
				sceneDepth = builder.CreateTexture(depthDesc);

				builder.SetView(kViewGeometry);
			},
			[&](const SRenderGraph& rg, bgfx::ViewId view)
			{
				bgfx::FrameBufferHandle fb = rg.GetFramebuffer(hdrColor);

				bgfx::setViewFrameBuffer(view, fb);
				bgfx::setViewRect(view, 0, 0, CurrentWindowWidth, CurrentWindowHeight);
				bgfx::setViewClear(view,
					BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
					0x303030ff, 1.0f, 0);

				// Bind shadow map as a sampler
				bgfx::TextureHandle shadowTex = rg.GetTexture(shadowMap);
				bgfx::setTexture(0, u_shadowMap, shadowTex);

				// TODO: set camera view/proj, iterate opaque objects
				//   bgfx::submit(view, g_geometryProgram);

				printf("[GeometryPass] submitted to view %u\n", (unsigned)view);
			}
		);

		// ── Pass 3: Tonemap ──────────────────────────────────────────────────────

		graph.AddPass("TonemapPass",
			[&](SPassBuilder& builder)
			{
				builder.ReadTexture(hdrColor);   // read HDR output of geometry pass
				builder.SetView(kViewTonemap);
				// No createTexture here — we write directly to the backbuffer.
				// Mark hdrColor as needed so it isn't culled.
			},
			[&](const SRenderGraph& rg, bgfx::ViewId view)
			{
				// Writing to the default back buffer — no setViewFrameBuffer needed.
				bgfx::setViewRect(view, 0, 0, CurrentWindowWidth, CurrentWindowHeight);
				bgfx::setViewClear(view, BGFX_CLEAR_NONE);

				bgfx::TextureHandle hdrTex = rg.GetTexture(hdrColor);
				bgfx::setTexture(0, u_hdrColor, hdrTex);

				SubmitFullscreenQuad(view, g_tonemapProgram);

				printf("[TonemapPass] submitted to view %u\n", (unsigned)view);
			}
		);

		// ── Compile + execute ────────────────────────────────────────────────────

		graph.Compile(CurrentWindowWidth, CurrentWindowHeight);
		graph.Execute();
	}*/

	void SRenderingProcessor::RenderDebugMessage(const SString& String, const float Duration /*= -1.f*/)
	{
		TextRenderer->ShowDebugText(String, Duration);
	}

	void SRenderingProcessor::Reset(const bool bForce)
	{
		SWindow* Window = gEngine->GetWindow();

		CurrentWindowWidth = Window->GetWindowWidth();
		CurrentWindowHeight = Window->GetWindowHeight();

		if (bForce)
		{
			bgfx::reset(CurrentWindowWidth, CurrentWindowHeight, BGFX_RESET_VSYNC);
		}
	}

	void SRenderingProcessor::RenderWorld(const SRenderView& ViewRef)
	{
		if (gEngine->IsInEditor())
		{
			SetupCameraRenderTransform(EditorCamera.Transform, EditorCamera.Data, ViewRef);
		}
		else
		{
			auto View = gEngine->GetWorld().GetRegistry().view<STransformComponent, SCameraComponent>();
			for (auto Entity : View)
			{
				const STransformComponent& TransformComp = View.get<STransformComponent>(Entity);
				const SCameraComponent& CameraComp = View.get<SCameraComponent>(Entity);
				SetupCameraRenderTransform(TransformComp.GetTransform(), CameraComp.CameraData, ViewRef);
				break;
			}
		}

		SetupCameraRenderTransform(ShadowCamera.Transform, ShadowCamera.Data, ShadowRenderView);

		Vec4 LightPos{ -1.f, -1.f, 0.f, 0.f };

		UniformManager.SetUniform("u_lightPos", &LightPos);

		const glm::mat4 LightViewMatrix = glm::lookAt(-Vec3{ LightPos }, Vec3{}, Vec::UpVector);
		const glm::mat4 LightProjectionMatrix = CalculateProjectionMatrix(ShadowCamera.Data, ShadowRenderView);

		bgfx::setViewTransform(ShadowRenderView.ViewId, &LightViewMatrix, &LightProjectionMatrix);

		const bgfx::Caps* Caps = bgfx::getCaps();
		const float sy = Caps->originBottomLeft ? 0.5f : -0.5f;
		const float sz = Caps->homogeneousDepth ? 0.5f : 1.0f;
		const float tz = Caps->homogeneousDepth ? 0.5f : 0.0f;
		
		const glm::mat4 MtxCrop =
		{
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f,   sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz,   0.0f,
			0.5f, 0.5f, tz,   1.0f,
		};

		const glm::mat4 MtxTmp = LightProjectionMatrix * MtxCrop;
		const glm::mat4 MtxShadow = LightViewMatrix * MtxTmp;

		auto ComponentView = gEngine->GetWorld().GetRegistry().view<STransformComponent, SMeshComponent>();
		ComponentView.each([this, &MtxShadow, &ViewRef](entt::entity Entity, const STransformComponent& TransformComponent, SMeshComponent& MeshComponent)
			{
				MeshComponent.MeshAsset->Load();
				if (!MeshComponent.MeshAsset->IsLoaded())
				{
					return;
				}

				const glm::mat4 TransformMatrix = TransformComponent.GetTransform().GetMatrix();

				const glm::mat4 LightMatrix = TransformMatrix * MtxShadow;

				{
					bgfx::setTransform(&TransformMatrix);
					bgfx::setVertexBuffer(0, MeshComponent.MeshAsset->GetVertexBufferHandle());
					bgfx::setIndexBuffer(MeshComponent.MeshAsset->GetIndexBufferHandle());
					bgfx::setState(BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW);

					UniformManager.SetUniform("u_lightMtx", &LightMatrix);

					for (const auto& UniformData : MeshComponent.GetUniformData())
					{
						UniformManager.SetUniform(UniformData.first, UniformData.second);
					}

					bgfx::submit(ShadowRenderView.ViewId, ShadowMapProgramHandle);
				}

				{
					bgfx::ProgramHandle ProgramHandle = ShaderProgramCache.GetProgramHandle(MeshComponent.ShaderProgramAsset);
					if (!bgfx::isValid(ProgramHandle))
					{
						return;
					}

					bgfx::setTransform(&TransformMatrix);
					bgfx::setVertexBuffer(0, MeshComponent.MeshAsset->GetVertexBufferHandle());
					bgfx::setIndexBuffer(MeshComponent.MeshAsset->GetIndexBufferHandle());
					bgfx::setTexture(0, UniformManager.GetUniformHandle("s_shadowMap"), ShadowRenderView.ViewTexture);
					bgfx::setState(0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_WRITE_Z
						| BGFX_STATE_DEPTH_TEST_LESS
						| BGFX_STATE_CULL_CCW);

					UniformManager.SetUniform("u_lightMtx", &LightMatrix);

					for (const auto& UniformData : MeshComponent.GetUniformData())
					{
						UniformManager.SetUniform(UniformData.first, UniformData.second);
					}

					bgfx::submit(ViewRef.ViewId, ProgramHandle);
				}

			});
	}

	void SRenderingProcessor::RenderUI(const SRenderView& ViewRef)
	{
		SetupCameraRenderTransform(UICamera.Transform, UICamera.Data, UIRenderView);

		TextRenderer->SubmitDebugText(ViewRef.ViewId);
	}

	void SRenderingProcessor::InitSceneView(SRenderView& ViewRef, const uint16 Width, const uint16 Height, const bool bDepthOnly)
	{
		if (bgfx::isValid(ViewRef.FrameBufferHandle))
		{
			bgfx::destroy(ViewRef.FrameBufferHandle);
		}

		ViewRef.Width = Width;
		ViewRef.Height = Height;

		std::vector<bgfx::TextureHandle> ViewTextures;

		if (!bDepthOnly)
		{
			bgfx::TextureHandle ColorTextureHandle = bgfx::createTexture2D(
				Width,
				Height,
				false,
				1,
				bgfx::TextureFormat::RGBA8,
				BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
			);

			ViewTextures.push_back(ColorTextureHandle);
		}

		bgfx::TextureFormat::Enum DepthFormat;
		if (IsTextureFormatSupported(bgfx::TextureFormat::D24S8))
		{
			DepthFormat = bgfx::TextureFormat::D24S8;
		}
		else if (IsTextureFormatSupported(bgfx::TextureFormat::D24))
		{
			DepthFormat = bgfx::TextureFormat::D24;
		}
		else
		{
			DepthFormat = bgfx::TextureFormat::D16;
		}
		
		bgfx::TextureHandle DepthTextureHandle = bgfx::createTexture2D(
			Width, 
			Height,
			false, 
			1,
			DepthFormat,
			BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
		);

		ViewTextures.push_back(DepthTextureHandle);

		constexpr bool bDestroyTexturesWithFrameBuffer = true;
		ViewRef.FrameBufferHandle = bgfx::createFrameBuffer(ViewTextures.size(), ViewTextures.data(), bDestroyTexturesWithFrameBuffer);
		ViewRef.ViewTexture = bgfx::getTexture(ViewRef.FrameBufferHandle, 0);
	}

	bool SRenderingProcessor::IsTextureFormatSupported(const bgfx::TextureFormat::Enum Format, const bool bAsFrameBuffer /*= false*/) const
	{
		const bgfx::Caps* Caps = bgfx::getCaps();
		uint16_t FormatSupport = Caps->formats[Format];

		if (bAsFrameBuffer)
		{
			return (FormatSupport & BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER) != 0;
		}

		return (FormatSupport & (BGFX_CAPS_FORMAT_TEXTURE_2D | BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED)) != 0;
	}

	void SRenderingProcessor::SetupCameraRenderTransform(const STransform& CameraTransform, const SCameraData& CameraData, const SRenderView& RenderView)
	{
		const Vec3 CameraPosition = CameraTransform.GetPosition();

		const Vec3 Forward = CameraTransform.GetForwardVector();
		const Vec3 LookAtTarget = CameraPosition + Forward;

		glm::mat4 ViewMatrix = glm::lookAt(CameraPosition, LookAtTarget, Vec::UpVector);

		const glm::mat4 ProjectionMatrix = CalculateProjectionMatrix(CameraData, RenderView);

		bgfx::setViewTransform(RenderView.ViewId, &ViewMatrix, &ProjectionMatrix);

		SWindow* Window = gEngine->GetWindow();

		const int32 WindowWidth = Window->GetWindowWidth();
		const int32 WindowHeight = Window->GetWindowHeight();
	}

	glm::mat4  SRenderingProcessor::CalculateProjectionMatrix(const SCameraData& CameraData, const SRenderView& RenderView) const
	{
		if (CameraData.Type == SCameraType::Orthographic)
		{
			return glm::ortho(CameraData.OrthographicArea.x, CameraData.OrthographicArea.y, CameraData.OrthographicArea.z, CameraData.OrthographicArea.w, CameraData.NearPlane, CameraData.FarPlane);
		}
		else
		{
			return glm::perspective(CameraData.FOV_Y, (float)RenderView.Width / (float)RenderView.Height, CameraData.NearPlane, CameraData.FarPlane);
		}
	}

	void SRenderingProcessor::SetFullscreenQuad()
	{



		if (3 == bgfx::getAvailTransientVertexBuffer(3, FSQVertexData::VertexLayout))
		{
			bgfx::TransientVertexBuffer VertexBuffer;
			bgfx::allocTransientVertexBuffer(&VertexBuffer, 3, FSQVertexData::VertexLayout);
			FSQVertexData* Vertex = (FSQVertexData*)VertexBuffer.data;

			Vertex[0] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f };
			Vertex[1] = { 3.0f, -1.0f, 0.0f, 2.0f, 0.0f };
			Vertex[2] = { -1.0f,  3.0f, 0.0f, 0.0f, 2.0f };

			bgfx::setVertexBuffer(0, &VertexBuffer);
		}
	}

}