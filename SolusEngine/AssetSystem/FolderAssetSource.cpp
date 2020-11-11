#include "FolderAssetSource.h"
#include "Asset.h"
#include "AssetSystem/AssetManager.h"

#include "Utility/Helper.h"

#include <iostream>

namespace Solus
{

	FolderAssetSource::FolderAssetSource(const std::string root)
		: AssetSource(root)
	{}

	void FolderAssetSource::Initialize()
	{
		Refresh();
	}

	void FolderAssetSource::Refresh()
	{
		FindAssetInFolder(root);
	}

	void FolderAssetSource::FindAssetInFolder(const fs::path folder)
	{
		for (auto& path : fs::recursive_directory_iterator(folder))
		{
			const fs::path& entryPath = path.path();
			if (path.is_directory())
			{
				FindAssetInFolder(entryPath);
			}
			else if (path.is_regular_file())
			{
				const std::string extension = entryPath.extension().string();
				if (StringCompareCase(extension, SAsset::ASSET_EXT))
				{
					const fs::path relativePath = fs::relative(entryPath, root);
					InitializeAsset(relativePath);
				}
			}
		}
	}

}
