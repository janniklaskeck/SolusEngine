#pragma once

#include "Utility/ClassMetaData.h"
#include "Object/SObject.h"
#include "Utility/SerializeUtil.h"
#include "AssetSystem/SAsset.h"

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

	void ClassMetaData::SetMemberAsset(const SObject* object, const std::string& name, SAsset* value)
	{
		if (!value)
			return;
		auto** assetPtr = GetMemberPtr<SAsset*>(object, name);
		*assetPtr = value;
	}

	void ClassMetaData::SetMemberAssetVector(const SObject* object, const std::string& name, const uint32_t index, SAsset* value)
	{
		if (!value)
			return;
		auto* assetVectorPtr = GetMemberPtr<std::vector<SAsset*>>(object, name);
		if (assetVectorPtr)
			(*assetVectorPtr)[index] = value;
	}

}
