#include "EditorSceneWindow.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "Render/RenderDevice.h"
#include "Utility/Timer.h"

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
		if (ImGui::Begin("Scene", nullptr, windowFlags))
		{
			if (ImGui::Button("SaveWorld"))
			{
				gEngine->GetWorld()->SaveToFile();
			}
			if (ImGui::Button("LoadWorld"))
			{
				gEngine->GetWorld()->ReadFromFile("c:\\Spiele\\test.txt");
			}
			sceneCamera->SetInputEnabled(ImGui::IsWindowFocused() && ImGui::IsMouseDown(1));
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 windowTopLeft = ImGui::GetCursorScreenPos();
			int width = (int)ImGui::GetWindowWidth();
			int height = (int)ImGui::GetWindowHeight();
			ImVec2 windowBottomRight = ImVec2(windowTopLeft.x + width, windowTopLeft.y + height);
			auto* renderDevice = gEngine->GetRenderDevice();
			auto texture = renderDevice->GetCurrentRenderSurface()->GetColorTexture();
			ImGui::GetWindowDrawList()->AddImage((void*)texture, windowTopLeft, windowBottomRight, ImVec2(0, 1), ImVec2(1, 0));
			auto fps = 1.f / gEngine->DeltaTime();
			auto deltaTime = gEngine->DeltaTime() * 1000.f;
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "TPS: %.2f, DeltaTime: %.2f ms", fps, deltaTime);
			fps = 1.f / gEngine->GetRenderTimer()->GetDeltaTime();
			deltaTime = gEngine->GetRenderTimer()->GetDeltaTime() * 1000.f;
			ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "FPS: %.2f, RenderDeltaTime: %.2f ms", fps, deltaTime);

			ImVec2 windowSize = ImGui::GetWindowSize();
			sceneCamera->UpdateProjectionMatrix(90.f, windowSize.x, windowSize.y);
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
