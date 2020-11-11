#include "AssetManager.h"

#include "AssetSystem/FolderAssetSource.h"

namespace Solus
{
	AssetManager::~AssetManager()
	{}

	void AssetManager::Initialize()
	{}

	void AssetManager::Update()
	{}

	void AssetManager::Destroy()
	{}

	void AssetManager::SetEngineAssetRoot(const std::string& engineAssetRoot)
	{
		engineAssetSource.reset(new FolderAssetSource(engineAssetRoot));
		engineAssetSource->Initialize();
	}

	void AssetManager::SetProjectAssetRoot(const std::string& projectAssetRoot)
	{
		if (!projectAssetRoot.empty())
		{
			projectAssetSource.reset(new FolderAssetSource(projectAssetRoot));
			projectAssetSource->Initialize();
		}
	}

	Asset Solus::AssetManager::GetAssetFromPath(const std::string path)
	{
		if (projectAssetSource)
		{
			Asset foundAsset = projectAssetSource->GetAssetFromPath(path);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		if (engineAssetSource)
		{
			Asset foundAsset = engineAssetSource->GetAssetFromPath(path);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		return Asset();
	}

	Asset AssetManager::GetAssetFromPath(const char* path)
	{
		return GetAssetFromPath(std::string(path));
	}

	Asset AssetManager::GetAsset(const SUUID assetId) const
	{
		if (projectAssetSource)
		{
			Asset foundAsset = projectAssetSource->GetAsset(assetId);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		if (engineAssetSource)
		{
			Asset foundAsset = engineAssetSource->GetAsset(assetId);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		return Asset();
	}

	AssetSource* AssetManager::GetEngineAssetSource() const
	{
		return engineAssetSource.get();
	}

	AssetSource* AssetManager::GetProjectAssetSource() const
	{
		return projectAssetSource.get();
	}

}
