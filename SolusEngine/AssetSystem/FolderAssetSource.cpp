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
		filesystem::path path(root);
		path = filesystem::absolute(path);
		filesystem::recursive_directory_iterator end;

		for (filesystem::recursive_directory_iterator i(path); i != end; ++i)
		{
			const filesystem::path cp = (*i);
			if (_stricmp(cp.extension().string().c_str(), ASSET_FILE_EXTENSION) > 0)
			{
				auto relativePath = filesystem::relative(cp, path);
				InitializeAsset(relativePath);
			}
		}
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
}
