#pragma once

#include "Engine/SolusEngine.h"

#include "AssetSystem/SAsset.h"
#include "AssetSystem/AssetSource.h"

#include "Utility/FileUtils.h"

namespace Solus
{

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

		SAsset* GetAssetFromPath(const std::string& path);
		SAsset* GetAsset(const SUUID assetId) const;
		AssetSource* GetEngineAssetSource() const;
		AssetSource* GetProjectAssetSource() const;

		const std::vector<std::string>& GetAssetFileTypeFilter() const;

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SAsset, T>, T*> ImportAsset(const fs::path filePath);

		SAsset* TryImportAsset(const fs::path filePath);

	private:
		std::string engineAssetRoot;

		std::unique_ptr<AssetSource> engineAssetSource;
		std::unique_ptr<AssetSource> projectAssetSource;

		std::vector<std::string> assetFileTypeFilters;
	};

	template<typename T>
	std::enable_if_t<std::is_base_of_v<SAsset, T>, T*> AssetManager::ImportAsset(const fs::path filePath)
	{
		T* asset = static_cast<T*>(GetAssetFromPath(filePath.string()));
		if (asset)
			return asset;

		const fs::path projectRelativePath = fs::relative(filePath, projectAssetSource->GetRootPath());
		if (projectRelativePath.root_name().string() == "Project")
		{
			T* importedAsset = SAsset::Import<T>(filePath);
			if (importedAsset)
			{
				projectAssetSource->InitializeAsset(importedAsset, projectRelativePath);
				return static_cast<T*>(GetAssetFromPath(projectRelativePath.string()));
			}
			return nullptr;
		}

		const fs::path engineRelativePath = fs::relative(filePath, engineAssetSource->GetRootPath());
		if (FileUtils::GetRootParentFolder(engineRelativePath) == "Editor")
		{
			T* importedAsset = SAsset::Import<T>(filePath);
			if (importedAsset)
			{
				engineAssetSource->InitializeAsset(importedAsset, engineRelativePath);
				return static_cast<T*>(GetAssetFromPath(engineRelativePath.string()));
			}
			return nullptr;
		}
		return nullptr;
	}

}
