#include "EditorMenuBar.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "Render/RenderDevice.h"
#include "Utility/Timer.h"

#include "IMGUI/imgui.h"

#include <algorithm>
#include <portable-file-dialogs.h>

namespace Solus
{
	void EditorMenuBar::Initialize()
	{}

	void EditorMenuBar::Render()
	{
		static std::shared_ptr<pfd::open_file> open_project_file;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project"))
				{
					open_project_file = std::make_shared<pfd::open_file>("Choose file", "C:\\");
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{
				}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X"))
				{
				}
				if (ImGui::MenuItem("Copy", "CTRL+C"))
				{
				}
				if (ImGui::MenuItem("Paste", "CTRL+V"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (open_project_file && open_project_file->ready())
		{
			auto result = open_project_file->result();
			if (result.size())
			{
				gEngine->SetCurrentProject(result[0]);
			}
			open_project_file = nullptr;
		}
	}

	void EditorMenuBar::OnFocusReceived()
	{}

	void EditorMenuBar::OnFocusLost()
	{}

	void EditorMenuBar::OnMinimized()
	{}

	void EditorMenuBar::OnMaximized()
	{}
}
