#pragma once

#include "SolusEngine.h"

#include <vector>
#include <string>

namespace Solus
{
	class Asset;
	class AssetSource;

	class SOLUS_API AssetManager
	{
	public:

		~AssetManager();

		void AddSource(AssetSource* source);

		Asset* GetAsset(std::string path);
		Asset* GetAsset(const char* path);

	private:
		std::vector<AssetSource*> sources;
	};

}
