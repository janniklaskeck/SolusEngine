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

	Asset* Solus::AssetManager::GetAsset(std::string path)
	{
		if (projectAssetSource)
		{
			Asset* foundAsset = projectAssetSource->GetAsset(path);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		if (engineAssetSource)
		{
			Asset* foundAsset = engineAssetSource->GetAsset(path);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		return nullptr;
	}

	Asset* AssetManager::GetAsset(const char* path)
	{
		return GetAsset(std::string(path));
	}

	Asset* AssetManager::GetAsset(const uint32_t assetId) const
	{
		return nullptr;
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
