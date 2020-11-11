#include "AssetMeta.h"
#include "Asset.h"
#include "Utility/FileUtils.h"
#include "Utility/Helper.h"

#include <filesystem>

namespace Solus
{
	AssetMeta::AssetMeta()
	{}

	void AssetMeta::Initialize(const SAsset* asset)
	{

	}

	void AssetMeta::ReadMetaData(const std::string& path)
	{
	}

	SUUID AssetMeta::GetMetaId() const
	{
		return SUUID();
	}

	void AssetMeta::WriteMetaData(const std::string& path)
	{
	}
}
