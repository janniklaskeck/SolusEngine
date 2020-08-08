#include "SolusObject.h"
#include "SolusObject.generated.h"
#include "Object/Entity.h"

#include "Utility/SerializeUtil.h"
#include "Engine/Engine.h"

namespace Solus
{

	SCLASS_IMPL(SolusObject);

	SolusObject::SolusObject()
	{}

	void SolusObject::Serialize(ArchiveStream& archive) const
	{
		ClassMetaData* metaDataPtr = GetClassMetaData();
		if (metaDataPtr)
			metaDataPtr->Serialize(&archive, this);
	}

	void SolusObject::Deserialize(ArchiveStream& archive)
	{
		ClassMetaData* metaDataPtr = GetClassMetaData();
		if (metaDataPtr)
			metaDataPtr->Deserialize(&archive, archive.root, this);
	}

	ClassMetaData* SolusObject::GetClassMetaData() const
	{
		const auto ptrType = get_type();
		const auto metaDataProperty = ptrType.get_property("MetaData");
		if (!metaDataProperty.is_valid())
		{
			gEngine->Log(LogLevel::LogError, "Could not get MetaData property for type: %.*s", ptrType.get_name().length(), ptrType.get_name().data());
			return nullptr;
		}
		const SolusObject* ptr = this;
		ClassMetaData* metaDataPtr = metaDataProperty.get_value(ptr).get_value<ClassMetaData*>();
		if (!metaDataPtr)
		{
			gEngine->Log(LogLevel::LogError, "ClassMetaData could not be retrieved from property for type: %.*s", ptrType.get_name().length(), ptrType.get_name().data());
			return nullptr;
		}
		return metaDataPtr;
	}

}
