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

		void AddSource(AssetSource* source);
		std::string GetEngineAssetRoot() const;
		void SetEngineAssetRoot(std::string& engineAssetRoot);

		Asset* GetAsset(std::string path);
		Asset* GetAsset(const char* path);

		size_t GetNumSources() const;
		AssetSource* GetAssetSource(size_t index) const;

	private:
		std::string engineAssetRoot;

		std::vector<AssetSource*> sources;
	};

}
