#include "render/text/STextRenderer.hpp"
#include "../../../include/assets/SAssetManager.hpp"
#include "../../../include/core/SEngine.hpp"
#include "../../../include/render/SWindow.hpp"

namespace Solus
{

	STextRenderer::STextRenderer(const SRenderingProcessor* ParentRenderer)
		: Renderer(ParentRenderer)
	{
		FontManager = std::make_unique<SFontManager>(512);
		TextBufferManager = std::make_unique<STextBufferManager>(FontManager.get());

		std::vector<uint8> Buffer;
		SAssetManager::Get().ReadFile("font/robotomono-regular.ttf", Buffer);
		m_fontAwesomeTtf = FontManager->createTtf(Buffer.data(), Buffer.size());

		m_fontAwesome72 = FontManager->createFontByPixelSize(m_fontAwesomeTtf, 0, 14);

		m_staticText = TextBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);
		m_transientText = TextBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);

		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"ASDASDASDQWEQWEQWE");

		TextBufferManager->setBackgroundColor(m_staticText, 0x551111ff);
		TextBufferManager->setUnderlineColor(m_staticText, 0xff2222ff);
		TextBufferManager->setOverlineColor(m_staticText, 0x2222ffff);
		TextBufferManager->setStrikeThroughColor(m_staticText, 0x22ff22ff);

		// Background.
		TextBufferManager->setStyle(m_staticText, STYLE_BACKGROUND);
		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"The quick ");

		// Strike-through.
		TextBufferManager->setStyle(m_staticText, STYLE_STRIKE_THROUGH);
		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"brown fox ");

		// Overline.
		TextBufferManager->setStyle(m_staticText, STYLE_OVERLINE);
		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"jumps over ");

		// Underline.
		TextBufferManager->setStyle(m_staticText, STYLE_UNDERLINE);
		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"the lazy ");

		// Background + strike-through.
		TextBufferManager->setStyle(m_staticText, STYLE_BACKGROUND | STYLE_STRIKE_THROUGH);
		TextBufferManager->appendText(m_staticText, m_fontAwesome72, L"dog\n");

		TextBufferManager->setStyle(m_staticText, STYLE_NORMAL);
	}

	STextRenderer::~STextRenderer()
	{
		FontManager->destroyTtf(m_fontAwesomeTtf);
		FontManager->destroyFont(m_fontAwesome72);

		TextBufferManager->destroyTextBuffer(m_staticText);
		TextBufferManager->destroyTextBuffer(m_transientText);
	}

	void STextRenderer::Tick(float DeltaTime)
	{

	}

	void STextRenderer::ShowDebugText(const SString& Text, const float Duration)
	{
		Messages.emplace_back(Text + "\n", Duration);
	}

	void STextRenderer::SubmitDebugText(const bgfx::ViewId ViewID)
	{
		TextBufferManager->clearTextBuffer(m_transientText);
		TextBufferManager->setPenPosition(m_transientText, 10.0f, 10.0f);

		// Iterate reversed so messages are ordered old->new from top to bottom
		for (int32 i = Messages.size() - 1; i >= 0; i--)
		{
			DebugMessage& Msg = Messages[i];
			TextBufferManager->appendText(m_transientText, m_fontAwesome72, Msg.Message.c_str());
			Msg.Duration -= gEngine->GetDeltaTime();
		}

		TextBufferManager->submitTextBuffer(m_transientText, ViewID);

		// Remove old messages only after submit so 0 duration messages are shown at least once
		Messages.erase(std::remove_if(
			Messages.begin(),
			Messages.end(),
			[](const DebugMessage& Msg) { return Msg.Duration <= 0.f; }
		), Messages.end());
	}
}