#pragma once

#include "Engine/SolusEngine.h"

#include "AssetSystem/Asset.h"
#include "AssetSystem/AssetSource.h"

#include "Utility/FileUtils.h"

namespace Solus
{
	class SAsset;

	class SOLUS_API AssetManager : public SubSystem
	{
		friend SAsset;
	public:
		~AssetManager();

		void Initialize() override;
		void Update() override;
		void Destroy() override;

		void SetEngineAssetRoot(const std::string& engineAssetRoot);
		void SetProjectAssetRoot(const std::string& projectAssetRoot);

		Asset GetAssetFromPath(const std::string& path);
		Asset GetAsset(const SUUID assetId) const;
		AssetSource* GetEngineAssetSource() const;
		AssetSource* GetProjectAssetSource() const;

		const std::vector<std::string>& GetAssetFileTypeFilter() const;

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SAsset, T>, Asset> ImportAsset(const fs::path filePath);

		Asset TryImportAsset(const fs::path filePath);

	private:
		std::string engineAssetRoot;

		std::unique_ptr<AssetSource> engineAssetSource;
		std::unique_ptr<AssetSource> projectAssetSource;

		std::vector<std::string> assetFileTypeFilters;
	};

	template<typename T>
	std::enable_if_t<std::is_base_of_v<SAsset, T>, Asset> AssetManager::ImportAsset(const fs::path filePath)
	{
		const Asset asset = GetAssetFromPath(filePath.string());
		if (asset.IsValid())
			return asset;

		const fs::path projectRelativePath = fs::relative(filePath, projectAssetSource->GetRootPath());
		if (projectRelativePath.root_name().string() == "Project")
		{
			T* importedAsset = SAsset::Import<T>(filePath);
			if (importedAsset)
			{
				projectAssetSource->InitializeAsset(importedAsset, projectRelativePath);
				return GetAssetFromPath(projectRelativePath.string());
			}
			return Asset();
		}

		const fs::path engineRelativePath = fs::relative(filePath, engineAssetSource->GetRootPath());
		if (FileUtils::GetRootParentFolder(engineRelativePath) == "Editor")
		{
			T* importedAsset = SAsset::Import<T>(filePath);
			if (importedAsset)
			{
				engineAssetSource->InitializeAsset(importedAsset, engineRelativePath);
				return GetAssetFromPath(engineRelativePath.string());
			}
			return Asset();
		}
		return Asset();
	}

}
