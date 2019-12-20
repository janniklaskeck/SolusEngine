#pragma once

#include "Engine/SolusEngine.h"

#include <vector>
#include <string>

namespace Solus
{
	class Asset;
	class AssetSource;

	class SOLUS_API AssetManager : public SubSystem
	{
	public:
		~AssetManager();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		void AddSource(AssetSource* source);

		Asset* GetAsset(std::string path);
		Asset* GetAsset(const char* path);

	private:
		std::vector<AssetSource*> sources;
	};

}
