#include "EditorAssetWindow.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "AssetSystem/AssetManager.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorAssetWindow::Initialize()
	{
		windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	}

	void EditorAssetWindow::Render()
	{
		auto size = gEngine->GetWindow()->GetWindowSize();
		ImGui::SetNextWindowPos(ImVec2(size.x * WINDOW_ASSET_POS_X, size.y * WINDOW_ASSET_POS_Y));
		ImGui::SetNextWindowSize(ImVec2(size.x * WINDOW_ASSET_SIZE_X, size.y * WINDOW_ASSET_SIZE_Y));
		if (ImGui::Begin("Assets", nullptr, windowFlags))
		{
			ImGui::BeginChild("FolderChild", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, ImGui::GetWindowHeight()-35), true);
			ImGui::Text("Folders");
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if (ImGui::TreeNode("Root"))
			{
				ImGui::Text("asdasdasd");
				ImGui::TreePop();
			}
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("FileChild", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.8f, ImGui::GetWindowHeight()-35), true);
			ImGui::Text("Files");
			{
				auto* manager = gEngine->GetAssetManager();
				ImVec2 buttonSize(80, 80);
				for (size_t i = 0; i < manager->GetNumSources(); i++)
				{
					auto* assetSource = manager->GetAssetSource(i);
					auto iter = assetSource->BeginIter();
					int count = 0;
					while (iter != assetSource->EndIter())
					{
						Asset* asset = iter->second;
						ImGui::Button(asset->GetFileName().c_str(), buttonSize);
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip(asset->GetFileName().c_str());
						if (count < 6)
						{
							ImGui::SameLine();
							count++;
						}
						else
							count = 0;
						iter++;
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void EditorAssetWindow::OnFocusReceived()
	{}

	void EditorAssetWindow::OnFocusLost()
	{}

	void EditorAssetWindow::OnMinimized()
	{}

	void EditorAssetWindow::OnMaximized()
	{}
}
