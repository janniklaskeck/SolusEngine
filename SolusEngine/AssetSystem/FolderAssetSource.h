#pragma once
#include "AssetSource.h"

#include <memory>

namespace Solus
{
	class Asset;

	struct SOLUS_API AssetFolder
	{
		AssetFolder(std::string folderName, AssetFolder* _parent = nullptr)
			: folderName(folderName), parent(_parent)
		{}

		std::filesystem::path GetFullRelativePath() const
		{
			if (!parent)
				return folderName;
			auto tmpPath = parent->GetFullRelativePath();
			tmpPath /= folderName;
			return tmpPath;
		}

		AssetFolder* parent;
		std::string folderName;
		std::vector<std::shared_ptr<AssetFolder>> childFolders;
	};

	class SOLUS_API FolderAssetSource : public AssetSource
	{
	public:
		FolderAssetSource(std::string root);

		void Initialize() override;

		Asset* GetAsset(std::string& path);

		AssetFolder* GetFolders() const
		{
			return sourceRoot.get();
		}

	private:
		void InitializeSubFolder(AssetFolder* subFolder, bool isRoot = false);
	private:
		std::unique_ptr<AssetFolder> sourceRoot;
	};

}
