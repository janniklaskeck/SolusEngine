#pragma once

#include "Engine/SolusEngine.h"

#include "AssetSystem/Asset.h"
#include "AssetSystem/AssetSource.h"

namespace Solus
{
	class SOLUS_API AssetManager : public SubSystem
	{
	public:
		~AssetManager();

		void Initialize() override;
		void Update() override;
		void Destroy() override;

		void SetEngineAssetRoot(const std::string& engineAssetRoot);
		void SetProjectAssetRoot(const std::string& projectAssetRoot);

		Asset GetAssetFromPath(const std::string path);
		Asset GetAssetFromPath(const char* path);
		Asset GetAsset(const SUUID assetId) const;
		AssetSource* GetEngineAssetSource() const;
		AssetSource* GetProjectAssetSource() const;


		template<typename T>
		std::enable_if_t<std::is_base_of_v<SAsset, T>, Asset> ImportAsset(const fs::path filePath);

	private:
		std::string engineAssetRoot;

		std::unique_ptr<AssetSource> engineAssetSource;
		std::unique_ptr<AssetSource> projectAssetSource;
	};

	template<typename T>
	std::enable_if_t<std::is_base_of_v<SAsset, T>, Asset>
		AssetManager::ImportAsset(const fs::path filePath)
	{
		if (SAsset::IsAssetFile(filePath))
			return GetAssetFromPath(filePath.string());

		SAsset* asset = SAsset::Import<T>(filePath);
		return Asset();
	}

}
