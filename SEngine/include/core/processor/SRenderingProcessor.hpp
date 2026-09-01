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
	class SRenderingProcessor final : public SProcessor
	{
	public:

		SRenderingProcessor();

		~SRenderingProcessor();

		void Tick(const float DeltaTime) override;

		int32 GetPriority() const override;

	public:

		void RenderDebugMessage(const SString& String, const float Duration = -1.f);

	private:


		// ── View IDs ──────────────────────────────────────────────────────────────────

		const bgfx::ViewId ViewID_Default = 0;
		const bgfx::ViewId ViewID_Shadow = 1;
		const bgfx::ViewId ViewID_UI = 64;

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

		uint16 ShadowMapSize = 512;

		bgfx::FrameBufferHandle ShadowMapFBHandle;
		bgfx::TextureHandle ShadowMapTextureHandle;

		bgfx::ProgramHandle ShadowMapProgramHandle;
		Ref<SShaderProgramAsset> ShadowMapProgramAsset;
	};
}
