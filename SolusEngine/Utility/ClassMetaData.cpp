#pragma once

#include "Utility/ClassMetaData.h"
#include "Object/SolusObject.h"
#include "Utility/SerializeUtil.h"

namespace Solus
{
    void ClassMetaData::Deserialize(ArchiveStream* archive, ArchiveEntry& entry, SolusObject* object)
	{
		ArchiveEntry nextEntry;
		while (archive->Deserialize(nextEntry, entry))
		{
			DeserializeMember(archive, object, nextEntry);
		}
	};
}
