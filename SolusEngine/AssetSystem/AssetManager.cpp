#include "AssetManager.h"

#include "AssetSystem/FolderAssetSource.h"
#include "AssetSystem/SAsset.h"

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

	Asset Solus::AssetManager::GetAssetFromPath(const std::string& path)
	{
		std::string actualPath = path;
		if (!SAsset::IsAssetFile(actualPath))
			actualPath.append(SAsset::ASSET_EXT);

		if (projectAssetSource)
		{
			const std::string projectSourceRootPath = projectAssetSource->GetRootPath().string();
			if (actualPath.rfind(projectSourceRootPath, 0) == 0)
				actualPath = actualPath.substr(projectSourceRootPath.size());
			Asset foundAsset = projectAssetSource->GetAssetFromPath(actualPath);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		if (engineAssetSource)
		{
			const std::string engineSourceRootPath = engineAssetSource->GetRootPath().string();
			if (actualPath.rfind(engineSourceRootPath, 0) == 0)
				actualPath = actualPath.substr(engineSourceRootPath.size() + 1);
			Asset foundAsset = engineAssetSource->GetAssetFromPath(actualPath);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		return Asset();
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
