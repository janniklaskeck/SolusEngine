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
		windowFlags = ImGuiWindowFlags_NoCollapse;
	}

	void EditorAssetWindow::Render()
	{
		if (ImGui::Begin("Assets", nullptr, windowFlags))
		{
			ImGui::BeginChild("FolderChild", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, ImGui::GetWindowHeight() - 35), true);
			ImGui::Text("Folders");

			auto source = gEngine->GetAssetManager()->GetAssetSource(0);
			FolderAssetSource* folderSource = (FolderAssetSource*)source;
			auto folders = folderSource->GetFolders();

			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			
			TreeDisplayFolder(folders);
			
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("FileChild", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.8f, ImGui::GetWindowHeight() - 35), true);
			RenderFiles();
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

	void EditorAssetWindow::TreeDisplayFolder(AssetFolder* folder)
	{
		bool isLeafFolder = folder->childFolders.size() == 0;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		std::string folderName = folder->folderName;
		if (folderName.empty())
		{
			folderName = "Root";
		}
		if (isLeafFolder)
		{
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}
		if (clickedFolder && clickedFolder->GetFullRelativePath() == folder->GetFullRelativePath())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool isFolderTreeOpen = ImGui::TreeNodeEx(folderName.c_str(), flags);

		if (ImGui::IsItemClicked())
		{
			if (clickedFolder != folder)
				SetClickedFolder(folder);
		}
		if (isFolderTreeOpen)
		{
			for (auto& subFolder : folder->childFolders)
			{
				TreeDisplayFolder(subFolder.get());
			}
			if (!isLeafFolder)
				ImGui::TreePop();
		}
	}

	void EditorAssetWindow::RenderFiles()
	{
		if (!clickedFolder)
			return;

		ImGui::Text("Files");
		{
			auto* manager = gEngine->GetAssetManager();
			ImVec2 buttonSize(80, 80);
			auto root = manager->GetAssetSource(0)->GetRootPath();
			std::filesystem::path path(root);
			path /= clickedFolder->GetFullRelativePath();
			std::filesystem::directory_iterator end;
			for (std::filesystem::directory_iterator i(path); i != end; ++i)
			{
				const std::filesystem::path cp = (*i);
				if (!std::filesystem::is_directory(cp))
				{
					if (_stricmp(cp.extension().string().c_str(), ASSET_FILE_EXTENSION) > 0)
					{
						auto relativePath = std::filesystem::relative(cp, root);
						auto* asset = manager->GetAsset(relativePath.string());
						if (asset)
						{
							ImGui::Button(asset->GetFileName().c_str(), buttonSize);
							if (ImGui::IsItemHovered())
								ImGui::SetTooltip(asset->GetFileName().c_str());
							ImGui::SameLine();
						}
					}
				}
			}

			/*for (size_t i = 0; i < manager->GetNumSources(); i++)
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
			}*/
		}
	}

	void EditorAssetWindow::SetClickedFolder(Solus::AssetFolder* folder)
	{
		clickedFolder = folder;
		gEngine->Log(LogLevel::LogDebug, "%s", folder->folderName.c_str());
	}
}
