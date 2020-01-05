#pragma once

#include <unordered_map>
#include <vector>
#include <typeinfo>

namespace Solus
{
	struct TypeMap
	{
		std::unordered_map<size_t, TypeInfo> types;
	};

	struct TypeInfo
	{
		const char* name;
		size_t size;

		struct Member
		{
			const char* name;
			size_t offset;
			TypeInfo* type;
		};

		std::vector<Member> members;
	};



#define REFLECT_ROOT() \
	static Solus::TypeMap typeInfos;

#define REFLECT_CLASS() \
	static Solus::TypeInfo typeInfo; \
	static void InitReflection(TypeInfo*);

#define REFLECT_CLASS_BEGIN(type) \
	static Solus::TypeInfo type::typeInfo{type::InitReflection}; \
	static void type::InitReflection(TypeInfo* info)\
	{ \
		using T = type; \
		info->name = #type; \
		info->size = sizeof(T); \
		Solus::typeInfos.types[typeid(T).hash_code()] = info;\
		info->members = { \

#define REFLECT_CLASS_MEMBER(name) \
			{#name, offsetof(T, name), Solus::TypeResolver<decltype(T::name)>::Get()}

#define REFLECT_CLASS_END() \
		}; \
}