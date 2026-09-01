#include "assets/SAsset.hpp"

#include "assets/SAssetManager.hpp"

namespace Solus
{

	void SAsset::Load()
	{
		if (IsLoaded() || !IsValid())
		{
			return;
		}

		SAssetManager::Get().ReadFile(GetPath(), Bytes);
	}

	void SAsset::Unload()
	{
		if (!IsLoaded())
		{
			return;
		}

		Bytes.clear();
	}

	bool SAsset::IsValid() const
	{
		return SAssetManager::Get().IsValidPath(Path);
	}

}
