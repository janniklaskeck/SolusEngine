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
		return Handle != SOLUS_INVALID_HANDLE;
	}

}
