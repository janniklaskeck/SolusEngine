#pragma once
#include "Engine/SolusEngine.h"

#include <functional>
#include <typeinfo>
#include <string>

namespace Solus
{

#define SOLUS_CLASS() // Empty macro to mark classes for reflection

#define SPROPERTY() // Empty macro to mark members for reflection

#define REFLECT(type) \
	public: \
	virtual size_t GetClassId(); \
	friend struct Solus::type##_Reflection; \
	static Solus::type##_Reflection Reflection;

#define SCLASS_IMPL(type) \
	size_t type::GetClassId() { \
		static size_t classHash = typeid(type).hash_code(); \
		return classHash; \
	} \
	Solus::type##_Reflection type::Reflection;

//#define ISA(instance) \
//this->
}

