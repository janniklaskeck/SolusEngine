#pragma once

#include "SubWindow.h"

#include "Render/FreeFlyCamera.h"
#include "AssetSystem/Asset.h"

namespace Solus
{
	class AssetSource;
	class FolderAssetSource;
	class AssetFolder;

	class EditorAssetWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		void Initialize() override;
		void Render() override;
		void OnFocusReceived() override;
		void OnFocusLost() override;
		void OnMinimized() override;
		void OnMaximized() override;
		// <--

	private:
		void RenderAssetSource(FolderAssetSource* source, const char* title);

		void RenderFiles();
		void RenderFile(const Asset& asset);

		//void SetClickedFolder(AssetFolder* folder);
		void SetClickedAsset(const Asset& asset);

		fs::path clickedFolderPath;
		Asset clickedAsset;

		struct AssetEntry
		{
			const std::string name;
			SUUID id;
		};

		struct AssetFolder
		{
			std::string name;
			fs::path relativePath;
			AssetFolder* parent = nullptr;
			std::vector<AssetFolder> folders;
			std::vector<AssetEntry> files;

			void AddAssetEntry(const fs::path& relativePathString, const SUUID& id)
			{
				const fs::path relativePath = relativePathString;
				if (!relativePath.has_parent_path())
				{
					AssetEntry newEntry = { relativePath.filename().string(), id };
					files.push_back(newEntry);
					return;
				}
				for (auto& pathPart : relativePath)
				{
					AssetFolder* folder = GetFolder(pathPart);
					if (folder)
					{
						fs::path newRelativePath = StripRootFolder(relativePath);
						folder->AddAssetEntry(newRelativePath, id);
						return;
					}
					const fs::path newRelativePath = GetParentRelativePath() / pathPart;
					folders.push_back({ pathPart.string(), newRelativePath, this });
					AddAssetEntry(relativePathString, id);
					return;
				}
			}

			void Clear()
			{
				folders.clear();
				files.clear();
			}

			AssetFolder* GetFolderByPath(const fs::path& relativePath)
			{
				for (AssetFolder& folder : folders)
				{
					if (folder.relativePath == relativePath)
						return &folder;
					AssetFolder* subFolder = folder.GetFolderByPath(relativePath);
					if (subFolder)
						return subFolder;
				}
				return nullptr;
			}

		private:

			AssetFolder* GetFolder(const fs::path& name)
			{
				for (AssetFolder& folder : folders)
				{
					if (folder.name == name.string())
						return &folder;
				}
				return nullptr;
			}

			const fs::path GetParentRelativePath()
			{
				if (!parent)
					return relativePath;
				return parent->GetParentRelativePath() / relativePath;
			}

			fs::path StripRootFolder(const fs::path& p)
			{
				const fs::path& parent_path = p.parent_path();
				if (parent_path.empty() || parent_path.string() == "/")
					return fs::path();
				return StripRootFolder(parent_path) / p.filename();
			}
		};

		AssetFolder rootFolder;

		void RenderAssetFolder(const AssetFolder& folder, const char* overrideName = nullptr);
	};

}