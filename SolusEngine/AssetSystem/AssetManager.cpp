#include "AssetManager.h"
#include "Asset.h"
#include "AssetSource.h"

namespace Solus
{
	AssetManager::~AssetManager()
	{
		for (auto* source : sources)
		{
			delete source;
		}
	}

	void Solus::AssetManager::AddSource(AssetSource* source)
	{
		if (!source)
			return;
		source->Initialize();
		sources.push_back(source);
	}

	Asset* Solus::AssetManager::GetAsset(std::string path)
	{
		for (auto* source : sources)
		{
			Asset* foundAsset = source->GetAsset(path);
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

}
