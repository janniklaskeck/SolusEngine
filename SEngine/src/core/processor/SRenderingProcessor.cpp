#include "core/processor/SRenderingProcessor.hpp"

#include "assets/SAssetManager.hpp"
#include "core/SEngine.hpp"
#include "core/SWorld.hpp"
#include "entity/component/SCameraComponent.hpp"
#include "entity/component/SMeshComponent.hpp"
#include "entity/component/STransformComponent.hpp"
#include "render/SRenderGraph.hpp"
#include "render/SWindow.hpp"
#include "render/text/STextRenderer.hpp"
#include "utils/font/font_manager.h"
#include "utils/font/text_buffer_manager.h"
#include "core/processor/SProcessorManager.hpp"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bx/string.h>
#include <SDL3/SDL.h>

#include <cstdio>
#include "../../../include/render/editor/SEditorInterface.hpp"

namespace Solus
{

	SRenderingProcessor::SRenderingProcessor()
	{
		SWindow* Window = gEngine->GetWindow();

		CurrentWindowWidth = Window->GetWindowWidth();
		CurrentWindowHeight = Window->GetWindowHeight();

		bgfx::Init init;
		init.platformData.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(Window->GetSDLWindow()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
		init.platformData.ndt = nullptr;
		init.type = bgfx::RendererType::Vulkan;
		init.resolution.width = CurrentWindowWidth;
		init.resolution.height = CurrentWindowHeight;
		init.resolution.reset = BGFX_RESET_VSYNC;

		if (!bgfx::init(init))
		{
			fprintf(stderr, "bgfx init failed\n");
			return;
		}

		bgfx::setDebug(BGFX_DEBUG_TEXT);

		

		TextRenderer = std::make_unique<STextRenderer>(this);

		UniformManager.CreateUniform("u_color", bgfx::UniformType::Vec4);
		UniformManager.CreateUniform("s_shadowMap", bgfx::UniformType::Sampler);
		UniformManager.CreateUniform("u_lightPos", bgfx::UniformType::Vec4);
		UniformManager.CreateUniform("u_lightMtx", bgfx::UniformType::Mat4);
		UniformManager.CreateUniform("u_depthScaleOffset", bgfx::UniformType::Vec4);

		ShadowMapProgramAsset = SAssetManager::Get().GetAsset<SShaderProgramAsset>();
		ShadowMapProgramAsset->SetPaths("shaders/vs_shadow.sc", "shaders/fs_shadow.sc");
		ShadowMapProgramHandle = ShaderProgramCache.GetProgramHandle(ShadowMapProgramAsset);

		// Get renderer capabilities info.
		const bgfx::Caps* caps = bgfx::getCaps();
		float depthScaleOffset[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
		if (caps->homogeneousDepth)
		{
			depthScaleOffset[0] = 0.5f;
			depthScaleOffset[1] = 0.5f;
		}

		UniformManager.SetUniform("u_depthScaleOffset", depthScaleOffset);

		ShadowMapTextureHandle = bgfx::createTexture2D(
			ShadowMapSize,
			ShadowMapSize,
			false,
			1,
			bgfx::TextureFormat::D16,
			BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
		);

		ShadowMapFBHandle = bgfx::createFrameBuffer(1, &ShadowMapTextureHandle, true);

		SEditorInterface::Init();
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
		SEditorInterface::RenderInterface(DeltaTime);

		bgfx::touch(0);

		Vec4 LightPos{ -1.f, -1.f, 0.f, 0.f };

		UniformManager.SetUniform("u_lightPos", &LightPos);

		float lightView[16];
		const bx::Vec3 eye = { -LightPos.x, -LightPos.y, LightPos.z };
		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		bx::mtxLookAt(lightView, eye, at);

		const bgfx::Caps* caps = bgfx::getCaps();
		float lightProj[16];
		const float area = 300.0f;
		bx::mtxOrtho(lightProj, -area, area, -area, area, -1000.0f, 1000.0f, 0.0f, caps->homogeneousDepth);

		bgfx::setViewTransform(ViewID_Shadow, lightView, lightProj);

		const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
		const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
		const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
		const float mtxCrop[16] =
		{
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f,   sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz,   0.0f,
			0.5f, 0.5f, tz,   1.0f,
		};

		float mtxTmp[16];
		bx::mtxMul(mtxTmp, lightProj, mtxCrop);
		float mtxShadow[16];
		bx::mtxMul(mtxShadow, lightView, mtxTmp);

		bgfx::setViewRect(ViewID_Shadow, 0, 0, ShadowMapSize, ShadowMapSize);
		bgfx::setViewFrameBuffer(ViewID_Shadow, ShadowMapFBHandle);

		// Clear backbuffer and shadowmap framebuffer at beginning.
		bgfx::setViewClear(ViewID_Shadow, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewClear(ViewID_Default, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		TextRenderer->Tick(DeltaTime);

		auto ComponentView = gEngine->GetWorld().GetRegistry().view<STransformComponent, SMeshComponent>();
		ComponentView.each([this, &mtxShadow](entt::entity Entity, const STransformComponent& TransformComponent, SMeshComponent& MeshComponent)
			{
				MeshComponent.MeshAsset->Load();
				if (!MeshComponent.MeshAsset->IsLoaded())
				{
					return;
				}

				auto TransformMatrix = TransformComponent.GetTransform().GetMatrix();

				float lightMtx[16];
				bx::mtxMul(lightMtx, (float*)&TransformMatrix, mtxShadow);
				{
					bgfx::setTransform(&TransformMatrix);
					bgfx::setVertexBuffer(0, MeshComponent.MeshAsset->GetVertexBufferHandle());
					bgfx::setIndexBuffer(MeshComponent.MeshAsset->GetIndexBufferHandle());
					bgfx::setState(BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW);

					UniformManager.SetUniform("u_lightMtx", lightMtx);

					for (const auto& UniformData : MeshComponent.GetUniformData())
					{
						UniformManager.SetUniform(UniformData.first, UniformData.second);
					}

					bgfx::submit(ViewID_Shadow, ShadowMapProgramHandle);
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
					bgfx::setTexture(0, UniformManager.GetUniformHandle("s_shadowMap"), ShadowMapTextureHandle);
					bgfx::setState(0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_WRITE_Z
						| BGFX_STATE_DEPTH_TEST_LESS
						| BGFX_STATE_CULL_CCW);

					UniformManager.SetUniform("u_lightMtx", lightMtx);

					for (const auto& UniformData : MeshComponent.GetUniformData())
					{
						UniformManager.SetUniform(UniformData.first, UniformData.second);
					}

					bgfx::submit(ViewID_Default, ProgramHandle);
				}
				
			});

		{
			const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
			const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };

			float view[16];
			bx::mtxLookAt(view, eye, at);

			// Setup a top-left ortho matrix for screen space drawing.
			const bgfx::Caps* caps = bgfx::getCaps();
			{
				float ortho[16];
				bx::mtxOrtho(
					ortho
					, 0.0f
					, float(CurrentWindowWidth)
					, float(CurrentWindowHeight)
					, 0.0f
					, 0.0f
					, 100.0f
					, 0.0f
					, caps->homogeneousDepth
				);
				bgfx::setViewTransform(ViewID_UI, view, ortho);
				bgfx::setViewRect(ViewID_UI, 0, 0, uint16_t(CurrentWindowWidth), uint16_t(CurrentWindowHeight));
			}

			TextRenderer->SubmitDebugText(ViewID_UI);
		}

		bgfx::frame();
		bgfx::dbgTextClear();
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

}