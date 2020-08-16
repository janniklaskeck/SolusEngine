#pragma once

#include "Window/SubWindow/SubWindow.h"
#include "Engine/Engine.h"
#include "Engine/LogListener.h"

#include "IMGUI/imgui.h"

namespace Solus
{


	class EditorLogWindow : public SubWindow, Solus::LogListener
	{
	public:

		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;

		virtual void OnLogReceived(Solus::LogLevel level, const char* message) override;

		void Clear();
		void AddLog(const char* fmt, ...);

	private:

		ImGuiTextBuffer Buf;
		ImGuiTextFilter Filter;
		ImVector<int> LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool AutoScroll = true;     // Keep scrolling if already at the bottom
	};

}
