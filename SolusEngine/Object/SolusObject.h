#pragma once

#include "Engine/SolusEngine.h"

#include "SolusObject.generated.h"

#include "Utility/RTTI.h"

namespace Solus
{
	class Entity;

	SOLUS_CLASS();
	class SOLUS_API SolusObject
	{
		REFLECT(SolusObject)
	public:
		virtual void Begin();
		virtual void End();
	private:
		static std::vector<TypeDescriptor_Struct*> typeInfos;
	public:
		static void Insert(size_t typeHash, TypeDescriptor_Struct* typeInfo);
		static TypeDescriptor_Struct* GetTypeInfo(size_t hash);
	};
}
