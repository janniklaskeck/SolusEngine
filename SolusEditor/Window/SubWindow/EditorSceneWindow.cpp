#include "EditorSceneWindow.h"

#include "Engine/Engine.h"

#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorSceneWindow::Initialize()
	{
		sceneCamera = gEngine->GetWorld()->SpawnEntity<Solus::FreeFlyCamera>(Vec3f(0, 0, -5), Vec3f(0, 0, 0));
		windowFlags = ImGuiWindowFlags_NoCollapse;
	}

	void EditorSceneWindow::Render()
	{
		auto* renderDevice = gEngine->GetRenderDevice();
		if (ImGui::Begin("Scene", nullptr, windowFlags))
		{
			sceneCamera->SetInputEnabled(ImGui::IsWindowFocused() && ImGui::IsMouseDown(1));
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 windowTopLeft = ImGui::GetCursorScreenPos();
			int width = (int)ImGui::GetWindowWidth();
			int height = (int)ImGui::GetWindowHeight();
			ImVec2 windowBottomRight = ImVec2(windowTopLeft.x + width, windowTopLeft.y + height);
			auto texture = renderDevice->GetCurrentRenderSurface()->GetColorTexture();
			ImGui::GetWindowDrawList()->AddImage((void*)texture, windowTopLeft, windowBottomRight, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "FPS: %.2f, DeltaTime: %.2f ms", 1.f / gEngine->DeltaTime(), gEngine->DeltaTime() * 1000.f);
		}
		ImGui::End();
	}

	void EditorSceneWindow::OnFocusReceived()
	{}

	void EditorSceneWindow::OnFocusLost()
	{}

	void EditorSceneWindow::OnMinimized()
	{}

	void EditorSceneWindow::OnMaximized()
	{}
}
