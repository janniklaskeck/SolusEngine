#include "EditorAssetWindow.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"
#include "AssetSystem/SAsset.h"
#include "Utils/UIUtils.h"

#include "IMGUI/imgui.h"

#include <magic_enum.hpp>
#include <algorithm>

namespace Solus
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

	void EditorAssetWindow::RenderAssetSource(FolderAssetSource* source, const char* title)
	{
		rootFolder.Clear();
		for (const auto& entry : source->pathAssets)
		{
			const std::string& relativePath = entry.first;
			const auto id = entry.second->GetAssetId();
			rootFolder.AddAssetEntry(relativePath, id);
		}
		RenderAssetFolder(rootFolder, title);
	}

	void EditorAssetWindow::RenderAssetFolder(const AssetFolder& folder, const char* overrideName /*= nullptr*/)
	{
		std::string folderName = folder.name;
		if (folderName.empty())
			folderName = overrideName;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		const bool isLeafFolder = folder.folders.size() == 0;
		if (isLeafFolder)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		const fs::path folderRelativePath = folder.relativePath;
		if (clickedFolderPath == folderRelativePath)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool isFolderTreeOpen = ImGui::TreeNodeEx(folderName.c_str(), flags);

		if (ImGui::IsItemClicked())
		{
			if (clickedFolderPath != folderRelativePath)
			{
				clickedFolderPath = folderRelativePath;
				//clickedAssetSource = folder->GetAssetSource();
				//folder->GetAssetSource()->Refresh();
			}
		}

		/*static UIUtils::PopupData data;

		std::string uniqueID = folderName + folderRelativePath;
		if (ImGui::BeginPopupContextItem(uniqueID.c_str()))
		{
			if (ImGui::Selectable("Create Folder"))
			{
				data.answerType = UIUtils::PopupAnswerType::OK_CANCEL;
				data.inputQuestion = "Folder Name";
				data.isModal = true;
				data.question = "Enter Folder Name: ";
				data.title = "Create new Folder in " + std::string(folderName);
				data.type = UIUtils::PopupType::STRING_INPUT;
				data.id = (void*)folder;
				data.okAction = [folder](std::string& str)
				{
					folder->CreateChildFolder(str);
					folder->GetAssetSource()->Refresh();
				};

				UIUtils::OpenPopup(data.title);
			}

			if (!folder->IsRootFolder() && ImGui::Selectable("Delete Folder"))
			{
				data.answerType = UIUtils::PopupAnswerType::OK_CANCEL;
				data.isModal = true;
				data.question = "Delete Folder?";
				data.title = "Delete Folder: " + std::string(folderName);
				data.type = UIUtils::PopupType::WARNING;
				data.id = (void*)folder;

				data.okAction = [folder](std::string& str)
				{
					folder->Delete();
					folder->GetAssetSource()->Refresh();
				};
				UIUtils::OpenPopup(data.title);
			}

			ImGui::EndPopup();
		}
		if (data.id && data.id == (void*)folder)
			UIUtils::RenderPopup(data);*/

		if (isFolderTreeOpen)
		{
			for (const AssetFolder& subFolder : folder.folders)
			{
				RenderAssetFolder(subFolder);
			}
			if (!isLeafFolder)
				ImGui::TreePop();
		}
	}

	void EditorAssetWindow::RenderFiles()
	{
		if (clickedFolderPath.empty())
			return;

		ImGui::Text("Files");
		auto* manager = gEngine->GetAssetManager();
		ImVec2 buttonSize(80, 80);
		AssetFolder* folder = rootFolder.GetFolderByPath(clickedFolderPath);
		if (!folder)
			return;
		for (const AssetEntry& entry : folder->files)
		{
			const Asset& asset = manager->GetAsset(entry.id);
			RenderFile(asset);
		}
	}

	void EditorAssetWindow::RenderFile(const Asset& asset)
	{
		const std::string filename = asset->GetFileName();
		ImGui::Selectable(filename.c_str(), asset == clickedAsset);
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("TESTDRAG", &asset, sizeof(Asset));
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemClicked())
		{
			if (clickedAsset != asset)
				SetClickedAsset(asset);
		}
		ImGui::SameLine(250);

		ImGui::Text(magic_enum::enum_name(asset->GetAssetType()).data());
		ImGui::SameLine(300);
		std::string sizeString;
		ImGui::Text(sizeString.c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(asset->GetFileName().c_str());
		ImGui::SameLine(420);
		ImGui::Text(asset->GetAssetId().ToString().c_str());
		ImGui::SameLine(435);
		ImGui::Text(std::to_string(asset->GetRefCount()).c_str());
	}

	//void EditorAssetWindow::SetClickedFolder(Solus::AssetFolder* folder)
	//{
	//}

	void EditorAssetWindow::SetClickedAsset(const Asset& asset)
	{
		clickedAsset = asset;
	}
}
