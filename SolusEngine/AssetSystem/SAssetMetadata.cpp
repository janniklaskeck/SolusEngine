#pragma once

#include "SAssetMetadata.h"

namespace Solus
{



	bool SAssetMetadata::Create(const fs::path filePath, const SAssetType assetType)
	{
		if (!FileUtils::CreateFile(filePath))
		{
			return false;
		}
		json newData;
		SUUID assetId;
		assetId.Create();
		newData["id"] = assetId.ToString();
		newData["type"] = (int)assetType;
		newData["data"] = json::object();
		return true;
	}

}
