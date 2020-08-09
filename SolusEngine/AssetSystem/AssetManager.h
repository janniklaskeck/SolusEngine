#pragma once

#include "Engine/SolusEngine.h"

#include "AssetSystem/Asset.h"
#include "AssetSystem/AssetSource.h"

#include <vector>
#include <string>

namespace Solus
{
	class SOLUS_API AssetManager : public SubSystem
	{
	public:
		~AssetManager();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		void SetEngineAssetRoot(const std::string& engineAssetRoot);
		void SetProjectAssetRoot(const std::string& projectAssetRoot);

		Asset* GetAsset(std::string path);
		Asset* GetAsset(const char* path);
		Asset* GetAsset(const uint32_t assetId) const;
		AssetSource* GetEngineAssetSource() const;
		AssetSource* GetProjectAssetSource() const;

	private:
		std::string engineAssetRoot;

		std::unique_ptr<AssetSource> engineAssetSource;
		std::unique_ptr<AssetSource> projectAssetSource;
	};

}
