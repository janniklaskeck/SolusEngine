#include "AssetManager.h"

#include "AssetSystem/FolderAssetSource.h"
#include "AssetSystem/ShaderAsset.h"
#include "AssetSystem/MeshAsset.h"
#include "AssetSystem/TextureAsset.h"

namespace Solus
{
	AssetManager::~AssetManager()
	{}

	void AssetManager::Initialize()
	{
		assetFileTypeFilters.push_back(MeshAsset::GetAssetFileTypeFilter().first);
		assetFileTypeFilters.push_back(MeshAsset::GetAssetFileTypeFilter().second);

		assetFileTypeFilters.push_back(TextureAsset::GetAssetFileTypeFilter().first);
		assetFileTypeFilters.push_back(TextureAsset::GetAssetFileTypeFilter().second);

		assetFileTypeFilters.push_back(ShaderAsset::GetAssetFileTypeFilter().first);
		assetFileTypeFilters.push_back(ShaderAsset::GetAssetFileTypeFilter().second);

		assetFileTypeFilters.push_back(SAsset::GetAssetFileTypeFilter().first);
		assetFileTypeFilters.push_back(SAsset::GetAssetFileTypeFilter().second);
	}

	void AssetManager::Update()
	{}

	void AssetManager::Destroy()
	{}

	void AssetManager::SetEngineAssetRoot(const std::string& engineAssetRoot)
	{
		engineAssetSource = std::make_unique<FolderAssetSource>(engineAssetRoot);
		engineAssetSource->Initialize();
	}

	void AssetManager::SetProjectAssetRoot(const std::string& projectAssetRoot)
	{
		if (!projectAssetRoot.empty())
		{
			projectAssetSource = std::make_unique<FolderAssetSource>(projectAssetRoot);
			projectAssetSource->Initialize();
		}
	}

	SAsset* AssetManager::GetAssetFromPath(const std::string& path)
	{
		std::string actualPath = path;
		if (!SAsset::IsAssetFile(actualPath))
			actualPath.append(SAsset::ASSET_EXT);

		if (projectAssetSource)
		{
			const std::string projectSourceRootPath = projectAssetSource->GetRootPath().string();
			if (actualPath.rfind(projectSourceRootPath, 0) == 0)
				actualPath = actualPath.substr(projectSourceRootPath.size());
			SAsset* foundAsset = projectAssetSource->GetAssetFromPath(actualPath);
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
			SAsset* foundAsset = engineAssetSource->GetAssetFromPath(actualPath);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		return nullptr;
	}

	SAsset* AssetManager::GetAsset(const SUUID assetId) const
	{
		if (projectAssetSource)
		{
			SAsset* foundAsset = projectAssetSource->GetAsset(assetId);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
		if (engineAssetSource)
		{
			SAsset* foundAsset = engineAssetSource->GetAsset(assetId);
			if (foundAsset)
			{
				return foundAsset;
			}
		}
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

	const std::vector<std::string>& AssetManager::GetAssetFileTypeFilter() const
	{
		return assetFileTypeFilters;
	}

	SAsset* AssetManager::TryImportAsset(const fs::path filePath)
	{
		std::string fileExtension = filePath.extension().string();
		ToLower(fileExtension);
		if (fileExtension == ".glsl")
		{
			return ImportAsset<ShaderAsset>(filePath);
		}
		if (fileExtension == ".obj")
		{
			return ImportAsset<MeshAsset>(filePath);
		}
		if (fileExtension == ".dds")
		{
			return ImportAsset<TextureAsset>(filePath);
		}

		return nullptr;
	}

}
