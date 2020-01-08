#pragma once
#include "Engine/SolusEngine.h"

#include <unordered_map>
#include <functional>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <cstddef>

namespace Solus
{

	struct SOLUS_API ClassMetaData
	{
	public:
		struct TypeInfo
		{
			const char* name = "";
			size_t size = 0;

			virtual void* GetValuePtr(void* object) = 0;
		};

		size_t id;
		const char* name;
		size_t size;
		ClassMetaData* parent = nullptr;
		std::unordered_map<std::string, TypeInfo*> data;

		template<typename V>
		V* GetValuePtr(const char* name, void* object)
		{
			if (data.find(name) == data.end())
				return nullptr;
			return (V*)data[name]->GetValuePtr(object);
		}
		static std::vector<ClassMetaData*> classMetaData;
		static void Insert(size_t id, ClassMetaData* metaData);
		static ClassMetaData* Get(size_t id);
	};

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
}

