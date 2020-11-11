#include "SObject.h"
#include "SObject.generated.h"
#include "Object/Entity.h"

#include "Utility/SerializeUtil.h"
#include "Engine/Engine.h"

namespace Solus
{

	SObject::SObject()
	{}

	void SObject::Serialize(ArchiveStream& archive) const
	{
		ClassMetaData* metaDataPtr = GetClassMetaData();
		if (metaDataPtr)
			metaDataPtr->Serialize(&archive, this);
	}

	void SObject::Deserialize(ArchiveStream& archive)
	{
		ClassMetaData* metaDataPtr = GetClassMetaData();
		if (metaDataPtr)
			metaDataPtr->Deserialize(&archive, archive.root, this);
	}

	ClassMetaData* SObject::GetClassMetaData() const
	{
		const auto ptrType = get_type();
		const auto metaDataProperty = ptrType.get_property("MetaData");
		if (!metaDataProperty.is_valid())
		{
			gEngine->Log(LogLevel::LogError, "Could not get MetaData property for type: %.*s", ptrType.get_name().length(), ptrType.get_name().data());
			return nullptr;
		}
		const SObject* ptr = this;
		ClassMetaData* metaDataPtr = metaDataProperty.get_value(ptr).get_value<ClassMetaData*>();
		if (!metaDataPtr)
		{
			gEngine->Log(LogLevel::LogError, "ClassMetaData could not be retrieved from property for type: %.*s", ptrType.get_name().length(), ptrType.get_name().data());
			return nullptr;
		}
		return metaDataPtr;
	}

}
