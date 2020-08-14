#include "EditorAssetWindow.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"
#include "Utils/UIUtils.h"

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
			int folderTreeWidth = (int)(ImGui::GetWindowContentRegionWidth() * 0.2f);
			if (folderTreeWidth < 200)
				folderTreeWidth = 200;
			ImGui::BeginChild("FolderChild", ImVec2((float)folderTreeWidth, ImGui::GetWindowHeight() - 35.f), true);

			AssetSource* projectSource = gEngine->GetAssetManager()->GetProjectAssetSource();
			if (projectSource)
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				RenderAssetSource((Solus::FolderAssetSource*)projectSource, "Project");
			}

			AssetSource* engineSource = gEngine->GetAssetManager()->GetEngineAssetSource();
			if (engineSource)
			{
				RenderAssetSource((Solus::FolderAssetSource*)engineSource, "Engine");
			}

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

	void EditorAssetWindow::RenderAssetSource(Solus::FolderAssetSource* source, const char* title)
	{
		RenderAssetFolder(source->GetRootFolder(), title);
	}

	void EditorAssetWindow::RenderAssetFolder(const Solus::AssetFolder* folder, const char* folderNameOverride /*= nullptr*/)
	{
		const char* folderName = folder->GetFolderName().c_str();
		if (folderNameOverride)
			folderName = folderNameOverride;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		std::vector<AssetFolder*> childFolders;
		folder->GetChildFolders(childFolders);
		const bool isLeafFolder = childFolders.size() == 0;
		if (isLeafFolder)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		const std::string folderRelativePath = folder->GetRelativePath().string();
		if (clickedFolderPath == folderRelativePath && clickedAssetSource == folder->GetAssetSource())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool isFolderTreeOpen = ImGui::TreeNodeEx(folderName, flags);

		if (ImGui::IsItemClicked())
		{
			if (clickedFolderPath != folderRelativePath || clickedAssetSource != folder->GetAssetSource())
			{
				clickedFolderPath = folderRelativePath;
				clickedAssetSource = folder->GetAssetSource();
				folder->GetAssetSource()->Refresh();
			}
		}
		
		const char* popupName = "CreateFolder...";
		bool createFolderClicked = false;
		static char buffer[32];
		std::string uniqueID = folderName + folderRelativePath;
		if (ImGui::BeginPopupContextItem(uniqueID.c_str()))
		{
			if (ImGui::Selectable("Create Folder"))
			{
				createFolderClicked = true;
				memset(buffer, 0, 32);
			}

			if (ImGui::Selectable("Delete Folder"))
			{
				folder->Delete();
				folder->GetAssetSource()->Refresh();
			}
			
			ImGui::EndPopup();
		}


		std::string uniqueID2 = folderName + folderRelativePath + "2";
		if (createFolderClicked)
			ImGui::OpenPopup(uniqueID2.c_str());
		if (ImGui::BeginPopupModal(uniqueID2.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();
			ImGui::InputText("Folder Name", buffer, IM_ARRAYSIZE(buffer));

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				folder->CreateChildFolder(buffer);
				folder->GetAssetSource()->Refresh();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				
			}
			ImGui::EndPopup();
		}

		if (isFolderTreeOpen)
		{
			for (const AssetFolder* subFolder : childFolders)
			{
				RenderAssetFolder(subFolder);
			}
			if (!isLeafFolder)
				ImGui::TreePop();
		}
	}

	void EditorAssetWindow::RenderFiles()
	{
		if (clickedFolderPath.empty() && !clickedAssetSource)
			return;

		ImGui::Text("Files");
		{
			auto* manager = gEngine->GetAssetManager();
			ImVec2 buttonSize(80, 80);
			auto root = clickedAssetSource->GetRootPath();
			std::filesystem::path path(root);
			path /= clickedFolderPath;
			if (std::filesystem::exists(path))
			{
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
								RenderFile(asset);
							}
						}
					}
				}
			}
		}
	}

	void EditorAssetWindow::RenderFile(Solus::Asset* asset)
	{
		ImGui::Selectable(asset->GetFileName(true).c_str(), asset == clickedAsset);
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("TESTDRAG", &asset, sizeof(Solus::Asset*));
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemClicked())
		{
			if (clickedAsset != asset)
				SetClickedAsset(asset);
		}
		ImGui::SameLine(250);
		ImGui::Text(asset->GetFileType().c_str());
		ImGui::SameLine(300);
		auto sizeString = UIUtils::ConvertSizeToString(asset->GetDataSize(true));
		ImGui::Text(sizeString.c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(asset->GetFileName().c_str());
		ImGui::SameLine(420);
			ImGui::Text(std::to_string(asset->GetAssetId()).c_str());
	}

	void EditorAssetWindow::SetClickedFolder(Solus::AssetFolder* folder)
	{
	}
	
	void EditorAssetWindow::SetClickedAsset(Solus::Asset* asset)
	{
		clickedAsset = asset;
	}
}
