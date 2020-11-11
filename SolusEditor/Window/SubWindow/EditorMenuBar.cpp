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
		static std::shared_ptr<pfd::open_file> open_file;
		static std::shared_ptr<pfd::save_file> save_file_as;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project"))
				{
					open_file = std::make_shared<pfd::open_file>("Choose file", "C:\\");
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Save", "CTRL+S"))
				{
					/*if (!gEngine->GetWorld()->SaveWorld())
					{
						save_file_as = std::make_shared<pfd::save_file>("Choose file", "C:\\");
					}*/
				}

				if (ImGui::MenuItem("Save As", "CTRL+Shift+S"))
				{
					save_file_as = std::make_shared<pfd::save_file>("Choose file", "C:\\");
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
				}

				// Disabled item
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (open_file && open_file->ready())
		{
			auto result = open_file->result();
			if (result.size())
			{
				gEngine->SetCurrentProject(result[0]);
			}
			open_file = nullptr;
		}
		if (save_file_as && save_file_as->ready())
		{
			auto result = save_file_as->result();
			if (result.size())
			{
				//gEngine->GetAssetManager()->GetProjectAssetSource()->CreateAsset(result);
				//gEngine->GetWorld()->SaveWorld();
			}
			save_file_as = nullptr;
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
