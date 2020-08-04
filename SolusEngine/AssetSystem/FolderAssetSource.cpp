#include "FolderAssetSource.h"
#include "Asset.h"

#include <filesystem>
#include <iostream>

using namespace std;

namespace Solus
{
	FolderAssetSource::FolderAssetSource(std::string root)
		: AssetSource(root)
	{}

	void FolderAssetSource::Initialize()
	{
		sourceRoot.reset(new AssetFolder(""));
		InitializeSubFolder(sourceRoot.get(), true); // Empty folder for root
	}

	Asset* FolderAssetSource::GetAsset(std::string& path)
	{
		auto pathString = filesystem::path(path).generic_string();
		if (assets.find(pathString) != assets.end())
		{
			return assets[pathString];
		}
		return nullptr;
	}

	void FolderAssetSource::InitializeSubFolder(AssetFolder* folder, bool isRoot)
	{
		if (!folder)
		{
			assert(folder);
			return;
		}
		filesystem::path path(root);
		path /= folder->GetFullRelativePath();
		filesystem::directory_iterator end;

		for (filesystem::directory_iterator i(path); i != end; ++i)
		{
			const filesystem::path cp = (*i);
			if (filesystem::is_directory(cp))
			{
				auto relativePath = cp.lexically_relative(path);
				std::shared_ptr<AssetFolder> newFolder(new AssetFolder(relativePath.string(), folder));
				folder->childFolders.push_back(newFolder);
				InitializeSubFolder(newFolder.get());
			}
			else
			{
				if (_stricmp(cp.extension().string().c_str(), ASSET_FILE_EXTENSION) > 0)
				{
					//subFolder->fileNames.push_back(cp.string().c_str());
					auto relativePath = filesystem::relative(cp, root);
					InitializeAsset(relativePath);
				}
			}
		}
	}
}
