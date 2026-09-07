#pragma once

#include "core/processor/SProcessor.hpp"
#include "render/SShaderProgramCache.hpp"
#include "render/SUniformManager.hpp"

#include <bgfx/bgfx.h>

struct SDL_Renderer;
class SFontManager;
class STextBufferManager;
namespace Solus { class STextRenderer; }

namespace Solus
{
	struct SRenderView
	{
		bgfx::FrameBufferHandle FrameBufferHandle = BGFX_INVALID_HANDLE;
		bgfx::TextureHandle ViewTexture = BGFX_INVALID_HANDLE;
		bgfx::ViewId ViewId;
		uint16 Width = 0;
		uint16 Height = 0;
	};

	class SRenderingProcessor final : public SProcessor
	{
	public:

		SRenderingProcessor();

		~SRenderingProcessor();

		void Tick(const float DeltaTime) override;

		int32 GetPriority() const override;

	public:

		void RenderDebugMessage(const SString& String, const float Duration = -1.f);

		void Reset(const bool bForce = true);

	private:

		void RenderWorld(const SRenderView& ViewRef);

		void RenderUI(const SRenderView& ViewRef);

		void InitSceneView(SRenderView& ViewRef, const uint16 Width, const uint16 Height, const bool bDepthOnly = false);

		bool IsTextureFormatSupported(const bgfx::TextureFormat::Enum Format, const bool bAsFrameBuffer = false) const;

		void SetupCameraRenderTransform(const STransform& CameraTransform, const SCameraData& CameraData, const SRenderView& RenderView);

		glm::mat4 CalculateProjectionMatrix(const SCameraData& CameraData, const SRenderView& RenderView) const;

	private:

		void SetFullscreenQuad();

	private:


		// ── View IDs ──────────────────────────────────────────────────────────────────

		const bgfx::ViewId ViewID_Default = 0;
		const bgfx::ViewId ViewID_Shadow = 8;
		const bgfx::ViewId ViewID_World = 16;
		const bgfx::ViewId ViewID_UI = 64;
		const bgfx::ViewId ViewID_Editor = 128;
		const bgfx::ViewId ViewID_ImGui = 255;

		//const bgfx::ViewId kViewShadow = 0;
		//const bgfx::ViewId kViewGeometry = 1;
		//const bgfx::ViewId kViewTonemap = 2;

		// ── Example shader / mesh handles (your app provides these) ──────────────────

		//bgfx::ProgramHandle g_shadowProgram = BGFX_INVALID_HANDLE;
		//bgfx::ProgramHandle g_geometryProgram = BGFX_INVALID_HANDLE;
		//bgfx::ProgramHandle g_tonemapProgram = BGFX_INVALID_HANDLE;
		//
		//// Uniform handles
		//bgfx::UniformHandle u_shadowMap = BGFX_INVALID_HANDLE;
		//bgfx::UniformHandle u_hdrColor = BGFX_INVALID_HANDLE;

	private:

		SShaderProgramCache ShaderProgramCache;

		SUniformManager UniformManager;

		std::unique_ptr<STextRenderer> TextRenderer;

		uint16 CurrentWindowWidth = 1;
		uint16 CurrentWindowHeight = 1;

		uint16 ShadowMapSize = 2048;

		bgfx::ProgramHandle ShadowMapProgramHandle;
		Ref<SShaderProgramAsset> ShadowMapProgramAsset;

		bgfx::ProgramHandle FullscreenQuadProgramHandle;
		Ref<SShaderProgramAsset> FullscreenQuadProgramAsset;

		SRenderView WorldRenderView;
		SRenderView ShadowRenderView;

		SRenderView UIRenderView;
		SRenderView EditorRenderView;

		SCamera EditorCamera;

		SCamera ShadowCamera;
		SCamera UICamera;

	};
}
