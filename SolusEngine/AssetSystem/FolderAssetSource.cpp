#include "FolderAssetSource.h"
#include "Asset.h"

#include <iostream>

namespace Solus
{
	AssetFolder::AssetFolder(AssetSource* parentSource, std::string folderName, AssetFolder* _parent)
		: source(parentSource), folderName(folderName), parent(_parent)
	{
		Initialize();
	}

	void AssetFolder::Initialize()
	{
		std::vector<filepath> childPaths;
		GetChildFolders(childPaths);

		for (const filepath path : childPaths)
		{
			auto newFolder = std::make_shared<AssetFolder>(source, path.string(), this);
			childFolders.push_back(newFolder);
		}

		childPaths.clear();
		GetChildFiles(childPaths);
		for (const filepath& path : childPaths)
		{
			if (_stricmp(path.extension().string().c_str(), ASSET_FILE_EXTENSION) > 0)
			{
				filepath relativePath = GetRelativePath() / path;
				source->InitializeAsset(relativePath);
			}
		}
	}

	filepath AssetFolder::GetRelativePath() const
	{
		if (!parent)
			return folderName;
		auto tmpPath = parent->GetRelativePath();
		tmpPath /= folderName;
		return tmpPath;
	}

	filepath AssetFolder::GetAbsolutePath() const
	{
		filepath path = source->GetRootPath();
		path /= GetRelativePath();
		return path;
	}

	const std::string& AssetFolder::GetFolderName() const
	{
		return folderName;
	}

	void AssetFolder::Refresh()
	{
		std::vector<filepath> childFolderPaths;
		GetChildFolders(childFolderPaths);

		std::vector<std::vector<std::shared_ptr<AssetFolder>>::const_iterator> deletedFolders;

		auto iter = childFolders.begin();
		for (; iter != childFolders.end(); iter++)
		{
			bool exists = false;
			for (const filepath path : childFolderPaths)
			{
				filepath path1 = iter->get()->GetFolderName();
				if (path1 == path)
				{
					exists = true;
					break;
				}
			}

			if (!exists)
			{
				deletedFolders.push_back(iter);
			}
		}

		for (std::vector<std::shared_ptr<AssetFolder>>::const_iterator& folder : deletedFolders)
		{
			childFolders.erase(folder);
		}

		for (const filepath path : childFolderPaths)
		{
			bool existsAlready = false;
			for (auto& folder : childFolders)
			{
				if (folder->GetFolderName() == path)
				{
					existsAlready = true;
					folder->Refresh();
					break;
				}
			}

			if (!existsAlready)
			{
				auto newFolder = std::make_shared<AssetFolder>(source, path.string(), this);
				childFolders.push_back(newFolder);
			}
		}
	}

	bool AssetFolder::Rename(const std::string& newName) const
	{
		std::filesystem::rename(GetAbsolutePath(), newName);
		return true;
	}

	bool AssetFolder::Delete() const
	{
		std::filesystem::remove_all(GetAbsolutePath());
		return false;
	}

	bool AssetFolder::CreateChildFolder(const std::string& childFolderName) const
	{
		return std::filesystem::create_directory(GetAbsolutePath() / childFolderName);
	}

	void AssetFolder::GetChildFolders(std::vector<AssetFolder*>& folders) const
	{
		for (auto& folder : childFolders)
		{
			folders.push_back(folder.get());
		}
	}

	AssetSource* AssetFolder::GetAssetSource() const
	{
		return source;
	}

	void AssetFolder::GetChildFolders(std::vector<filepath>& childFolders)
	{
		filepath path(GetAbsolutePath());
		std::filesystem::directory_iterator end;

		for (std::filesystem::directory_iterator i(path); i != end; ++i)
		{
			const filepath cp = (*i);
			if (std::filesystem::is_directory(cp))
			{
				childFolders.push_back(cp.lexically_relative(path));
			}
		}
	}

	void AssetFolder::GetChildFiles(std::vector<filepath>& childFiles)
	{
		filepath path(GetAbsolutePath());
		std::filesystem::directory_iterator end;

		for (std::filesystem::directory_iterator i(path); i != end; ++i)
		{
			const filepath cp = (*i);
			if (std::filesystem::is_regular_file(cp))
			{
				childFiles.push_back(cp.lexically_relative(path));
			}
		}
	}

	FolderAssetSource::FolderAssetSource(std::string root)
		: AssetSource(root)
	{}

	void FolderAssetSource::Initialize()
	{
		sourceRoot.reset(new AssetFolder(this, ""));
	}

	Asset* FolderAssetSource::GetAsset(std::string& path)
	{
		auto pathString = filepath(path).generic_string();
		if (assets.find(pathString) != assets.end())
		{
			return assets[pathString];
		}
		return nullptr;
	}

	void FolderAssetSource::Refresh()
	{
		sourceRoot->Refresh();
	}

}
