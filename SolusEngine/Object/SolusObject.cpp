#include "SolusObject.h"

#include "Object/Entity.h"

namespace Solus
{
	std::vector<TypeDescriptor_Struct*> SolusObject::typeInfos;

	REFLECT_STRUCT_BEGIN_EMPTY(SolusObject)
		REFLECT_STRUCT_END();

	void SolusObject::Insert(size_t typeHash, TypeDescriptor_Struct* typeInfo)
	{
		typeInfo->hash = typeHash;
		typeInfos.push_back(typeInfo);
	}

	TypeDescriptor_Struct* SolusObject::GetTypeInfo(size_t hash)
	{
		for (auto* info : typeInfos)
		{
			if (info->hash == hash)
			{
				return info;
			}
		}
		return nullptr;
	}

	void SolusObject::Begin()
	{

	}

	void SolusObject::End()
	{

	}
}
