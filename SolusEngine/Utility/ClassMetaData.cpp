#pragma once

#include "Utility/ClassMetaData.h"
#include "Object/SolusObject.h"

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

	ClassMetaData* ClassMetaData::GetClassMetaData(void* object)
	{
		auto* solusObject = (SolusObject*)(object);
		if (!solusObject)
			return nullptr;
		size_t classId = solusObject->GetClassId();
		for (auto* classData : classMetaData)
		{
			if (classData->id == classId)
				return classData;
		}
		return nullptr;
	}
	
	ClassMetaData::TypeInfo* ClassMetaData::GetTypeInfo(const char* name, ClassMetaData* metaData)
	{
		if (metaData->data.find(name) != metaData->data.end())
		{
			return metaData->data[name];
		}
		if (metaData->parents.size() > 0)
		{
			for (auto* parentMetaData : metaData->parents)
			{
				auto* parentTypeInfo = GetTypeInfo(name, parentMetaData);
				if (parentTypeInfo)
					return parentTypeInfo;
			}
		}
		return nullptr;
	}
}
