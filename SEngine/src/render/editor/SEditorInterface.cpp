#include "render/editor/SEditorInterface.hpp"

#include "render/imgui/imgui_bgfx.hpp"
#include "core/processor/SInputProcessor.hpp"
#include "../../../include/render/SWindow.hpp"

namespace Solus
{



	void SEditorInterface::Init()
	{
		imguiCreate();
	}

	void SEditorInterface::Destroy()
	{
		imguiDestroy();
	}

	void SEditorInterface::RenderInterface(float DeltaTime)
	{
		const SInputProcessor& InputProcessor = gEngine->GetProcessor<SInputProcessor>();
		const Vec2 MousePos = InputProcessor.GetMousePos();
		const int32 MouseWheelDelta = InputProcessor.GetMouseWheelDelta();

		SWindow* Window = gEngine->GetWindow();

		imguiBeginFrame(
			MousePos.x,
			MousePos.y,
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::LeftButton) ? IMGUI_MBUT_LEFT : 0 |
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::RightButton) ? IMGUI_MBUT_RIGHT : 0 |
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::MiddleButton) ? IMGUI_MBUT_MIDDLE : 0,
			MouseWheelDelta,
			Window->GetWindowWidth(),
			Window->GetWindowHeight());

		ImGui::ShowDemoWindow();

		ImGui::GetWindowDrawList()->AddImage();

		imguiEndFrame();
	}

}