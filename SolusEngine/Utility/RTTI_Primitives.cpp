#include "RTTI.h"

#include "Vector.h"

namespace Solus
{
	std::vector<ClassMetaData*> ClassMetaData::classMetaData;

	void ClassMetaData::Insert(size_t id, ClassMetaData* metaData)
	{
		metaData->id = id;
		classMetaData.push_back(metaData);
	}

	ClassMetaData* ClassMetaData::Get(size_t id)
	{
		for (auto* info : classMetaData)
		{
			if (info->id == id)
			{
				return info;
			}
		}
		return nullptr;
	}
}
