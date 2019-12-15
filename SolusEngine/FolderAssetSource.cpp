#include "FolderAssetSource.h"
#include "Asset.h"

#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost;

namespace Solus
{
	FolderAssetSource::FolderAssetSource(std::string root)
		: AssetSource(root)
	{}

	void FolderAssetSource::Initialize()
	{
		filesystem::path path(root);
		filesystem::recursive_directory_iterator end;

		for (filesystem::recursive_directory_iterator i(path); i != end; ++i)
		{
			const filesystem::path cp = (*i);
			if (_stricmp(cp.extension().string().c_str(), Asset::ASSET_FILE_EXTENSION) > 0)
			{
				std::string filePath = cp.relative_path().string();
				InitializeAsset(filePath, cp.extension().string());
			}
		}
	}

	Asset* FolderAssetSource::GetAsset(std::string& path)
	{
		if (assets.find(path) != assets.end())
		{
			return assets[path];
		}
		return nullptr;
	}
}
