#pragma once
#include "Engine/SolusEngine.h"

#include <rttr/registration.h>
#include <rttr/registration_friend>

#include <functional>
#include <typeinfo>
#include <string>

namespace Solus
{

#define SOLUS_CLASS() // Empty macro to mark classes for reflection

#define SPROPERTY() // Empty macro to mark members for reflection
	
#define META_ROOT(type) \
	RTTR_ENABLE() \
	RTTR_REGISTRATION_FRIEND \
	public: \
	friend struct Solus::type##_ClassMetaData; \
	static Solus::type##_ClassMetaData* MetaData; \
	virtual const std::string GetTypeName() const { return #type; } \

#define META(type, parentClass) \
	RTTR_ENABLE(parentClass) \
	RTTR_REGISTRATION_FRIEND \
	public: \
	friend struct Solus::type##_ClassMetaData; \
	static Solus::type##_ClassMetaData* MetaData; \
	virtual const std::string GetTypeName() const { return #type; } \
		

#define REFLECT(type) \
	public: \
	virtual size_t GetClassId() const; \
	friend struct Solus::type##_Reflection; \
	static Solus::type##_Reflection Reflection;
}

