#pragma once

#include "utils/font/font_manager.h"
#include "utils/font/text_buffer_manager.h"

#include "bgfx/bgfx.h"

namespace Solus
{
	class SRenderingProcessor;

	class STextRenderer
	{
	public:

		STextRenderer(const SRenderingProcessor* Renderer);

		~STextRenderer();

	public:

		void Tick(float DeltaTime);

		void ShowDebugText(const SString& Text, const float Duration = -1.f);

		void SubmitDebugText(const bgfx::ViewId ViewID);

	private:

		struct DebugMessage
		{
			std::string Message;
			float Duration = -1.f;
		};

		std::vector<DebugMessage> Messages;

		std::unique_ptr<SFontManager> FontManager;
		std::unique_ptr<STextBufferManager> TextBufferManager;

		const SRenderingProcessor* Renderer = nullptr;

		TrueTypeHandle m_fontAwesomeTtf;
		FontHandle m_fontAwesome72;

		TextBufferHandle m_transientText;
		TextBufferHandle m_staticText;
	};
}