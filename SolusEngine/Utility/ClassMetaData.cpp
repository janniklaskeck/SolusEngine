#pragma once

#include "Utility/ClassMetaData.h"
#include "Object/SObject.h"
#include "Utility/SerializeUtil.h"
#include "AssetSystem/Asset.h"

namespace Solus
{
	void ClassMetaData::Deserialize(ArchiveStream* archive, ArchiveEntry& entry, SObject* object)
	{
		ArchiveEntry nextEntry;
		while (archive->Deserialize(nextEntry, entry))
		{
			DeserializeMember(archive, object, nextEntry);
		}
	};

	void ClassMetaData::SetMemberAsset(const SObject* object, const std::string& name, Asset* value)
	{
		if (!value)
			return;
		Asset* assetPtr = GetMemberPtr<Asset>(object, name);
		if (assetPtr)
			assetPtr->Set(value->Get());
	}

	void ClassMetaData::SetMemberAssetVector(const SObject* object, const std::string& name, const uint32_t index, Asset* value)
	{
		if (!value)
			return;
		auto* assetVectorPtr = GetMemberPtr<std::vector<Asset>>(object, name);
		if (assetVectorPtr)
			(*assetVectorPtr)[index].Set(value->Get());
	}

}
