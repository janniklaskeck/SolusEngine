#include "AssetSource.h"
#include "Engine/Engine.h"

#include "AssetSystem/SAsset.h"

namespace Solus
{

	AssetSource::AssetSource(std::string root)
	{
		this->root = std::filesystem::absolute(root);
	}

	SAsset* AssetSource::GetAssetFromPath(const std::string& path)
	{
		std::string pathString = fs::path(path).string();
		ReplaceChar(pathString, '/', '\\');
		if (pathAssets.find(pathString) != pathAssets.end())
		{
			return pathAssets[pathString];
		}
		return nullptr;
	}

	SAsset* AssetSource::GetAsset(const SUUID id)
	{
		if (idAssets.find(id) != idAssets.end())
			return idAssets[id];
		return nullptr;
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
		idAssets[importedAsset->GetAssetId()] = importedAsset;
		pathAssets[relativePath.string()] = importedAsset;
	}

	void AssetSource::InitializeAsset(SAsset* importedAsset, const fs::path relativePath)
	{
		if (!importedAsset)
			return;
		idAssets[importedAsset->GetAssetId()] = importedAsset;
		pathAssets[relativePath.string()] = importedAsset;
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
