#pragma once

#include "Engine/SolusEngine.h"
#include "SObject.generated.h"

#include "Utility/RTTI.h"

namespace Solus
{

	SOLUS_CLASS();
	class SOLUS_API SObject
	{
		META_ROOT(SObject)
	public:
		SObject();

		void Serialize(ArchiveStream& archive) const;

		void Deserialize(ArchiveStream& archive);

		ClassMetaData* GetClassMetaData() const;
	};


}
