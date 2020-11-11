#pragma once

#include "Utility/ClassMetaData.h"
#include "Object/SObject.h"
#include "Utility/SerializeUtil.h"

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
}
