#pragma once

#include "Engine/SolusEngine.h"
#include "SolusObject.generated.h"

#include "Utility/RTTI.h"

namespace Solus
{

	SOLUS_CLASS();
	class SOLUS_API SolusObject
	{
		META_ROOT(SolusObject)
	public:
		SolusObject();

		void Serialize(ArchiveStream& archive) const;

		void Deserialize(ArchiveStream& archive);

		virtual void PostSerialize()
		{}

		ClassMetaData* GetClassMetaData() const;
	};


}
