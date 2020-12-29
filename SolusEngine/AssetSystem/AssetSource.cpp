#include "AssetSource.h"
#include "AssetSource.h"
#include "Engine/Engine.h"

#include "AssetSystem/SAsset.h"

namespace Solus
{

	AssetSource::AssetSource(std::string root)
	{
		this->root = std::filesystem::absolute(root);
	}

	Asset AssetSource::GetAssetFromPath(const std::string& path)
	{
		std::string pathString = fs::path(path).string();
		ReplaceChar(pathString, '/', '\\');
		if (pathAssets.find(pathString) != pathAssets.end())
		{
			return pathAssets[pathString];
		}
		return Asset();
	}

	Asset AssetSource::GetAsset(const SUUID id)
	{
		if (idAssets.find(id) != idAssets.end())
			return idAssets[id];
		return Asset();
	}

	const fs::path& AssetSource::GetRootPath() const
	{
		return root;
	}

	void AssetSource::InitializeAsset(const fs::path relativePath)
	{
		auto absolutePath = root / relativePath;
		SAsset* importedAsset = SAsset::Import(absolutePath);
		if (!importedAsset)
			return;
		Asset asset;
		asset.Set(importedAsset);
		idAssets[asset.GetId()] = asset;
		pathAssets[relativePath.string()] = asset;
	}

	void AssetSource::InitializeAsset(SAsset* importedAsset, const fs::path relativePath)
	{
		if (!importedAsset)
			return;
		auto absolutePath = root / relativePath;
		Asset asset;
		asset.Set(importedAsset);
		idAssets[asset.GetId()] = asset;
		pathAssets[relativePath.string()] = asset;
	}

	void AssetSource::CleanPath(std::string& path)
	{
		if (path.rfind("./", 0) != std::string::npos)
		{
			path.erase(0, 2);
		}
		std::replace(path.begin(), path.end(), '\\', '/');
	}

}
