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
	template<typename T>
	struct SOLUS_API ClassMetaData
	{
	protected:
		std::unordered_map<std::string, std::function<void* (T*)>> functions;
	public:
		template<typename V>
		V* GetValuePtr(const char* name, T* object)
		{
			if (functions.find(name) == functions.end())
				return nullptr;
			return (V*)functions[name](object);
		}
	};

	struct SOLUS_API TypeDescriptor
	{
		const char* name;
		size_t size;

		TypeDescriptor(const char* name, size_t size)
			: name(name), size(size)
		{}

		virtual ~TypeDescriptor()
		{}
		virtual std::string GetName() const
		{
			return name;
		}
		virtual void Print(const void* object, size_t indentLevel = 0) const = 0;
	};

	template<typename T>
	TypeDescriptor* GetPrimitiveDescriptor();

	struct SOLUS_API DefaultResolver
	{
		template<typename T> static char func(decltype(&T::Reflection));
		template<typename T> static int func(...);
		template<typename T>
		struct IsReflected
		{
			enum
			{
				value = (sizeof(func<T>(nullptr)) == sizeof(char))
			};
		};
		template<typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* Get()
		{
			return &T::Reflection;
		}

		template<typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* Get()
		{
			return GetPrimitiveDescriptor<T>();
		}
	};

	template<typename T>
	struct SOLUS_API TypeResolver
	{
		static TypeDescriptor* Get()
		{
			return DefaultResolver::Get<T>();
		}
	};

	struct SOLUS_API TypeDescriptor_Struct : TypeDescriptor
	{
		struct Member
		{
			const char* name;
			size_t offset;
			TypeDescriptor* type;
		};
		TypeDescriptor_Struct* parent = nullptr;
		size_t hash = 0;
		std::vector<Member> members;

		TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*))
			: TypeDescriptor{ nullptr, 0 }
		{
			init(this);
		}

		TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init)
			: TypeDescriptor{ nullptr, 0 }, members{ init }
		{}

		const Member& GetMember(const char* name) const
		{
			for (auto& member : members)
			{
				if (_stricmp(member.name, name) == 0)
				{
					return member;
				}
			}
			static Member emptyMember{ nullptr, 0, nullptr };
			return emptyMember;
		}

		template<typename T>
		T GetValue(const void* object, const char* name) const
		{
			const auto member = GetMember(name);
			if (member.name)
			{
				return (T) * ((char*)object + member.offset);
			}
			return 0;
		}

		template<typename T>
		T* GetValuePtr(const void* object, const char* name) const
		{
			const auto member = GetMember(name);
			if (member.name)
			{
				return (T*)((char*)object + member.offset);
			}
			return nullptr;
		}

		virtual void Print(const void* object, size_t indentLevel) const override
		{
			std::cout << name << " {" << std::endl;
			for (const Member& member : members)
			{
				std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
				member.type->Print((char*)object + member.offset, indentLevel + 1);
				std::cout << std::endl;
			}
			std::cout << std::string(4 * indentLevel, ' ') << "}";
		}
	};

#define REFLECT_ROOT() \
	public: \
	static std::unordered_map<size_t, Solus::TypeDescriptor_Struct*> typeInfos; \
	static Solus::TypeDescriptor_Struct* GetTypeInfo(Solus::Entity*);

#define REFLECT_ROOT_IMPL() \
	std::unordered_map<size_t, Solus::TypeDescriptor_Struct*> Solus::SolusObject::typeInfos; \
	Solus::TypeDescriptor_Struct* SolusObject::GetTypeInfo(Solus::Entity* entity) \
	{\
		return Solus::SolusObject::typeInfos[typeid(*entity).hash_code()]; \
	}

#define SOLUS_CLASS() // Empty macro to mark classes for reflection

#define SPROPERTY() // Empty macro to mark members for reflection

#define REFLECT(type) \
	public: \
	virtual size_t GetClassId(); \
	friend struct Solus::DefaultResolver; \
	friend struct Solus::type##_Reflection; \
	static Solus::type##_Reflection Reflection_Acc; \
	static Solus::TypeDescriptor_Struct Reflection; \
	static void InitReflection(Solus::TypeDescriptor_Struct*);

#define REFLECT_STRUCT_BEGIN_EMPTY(type) \
	size_t type::GetClassId() { \
		static size_t classHash = typeid(type).hash_code(); \
		return classHash; \
	} \
	Solus::type##_Reflection type::Reflection_Acc; \
	Solus::TypeDescriptor_Struct type::Reflection{type::InitReflection}; \
	void type::InitReflection(Solus::TypeDescriptor_Struct* typeDesc) { \
		using T = type; \
		typeDesc->parent = nullptr; \
		typeDesc->name = #type; \
		typeDesc->size = sizeof(T); \
		typeDesc->members = {

#define REFLECT_STRUCT_BEGIN(type, parentType) \
	size_t type::GetClassId() { \
		static size_t classHash = typeid(type).hash_code(); \
		return classHash; \
	} \
	Solus::type##_Reflection type::Reflection_Acc; \
	Solus::TypeDescriptor_Struct type::Reflection{type::InitReflection}; \
	void type::InitReflection(Solus::TypeDescriptor_Struct* typeDesc) { \
		using T = type; \
		typeDesc->parent = &parentType::Reflection; \
		typeDesc->name = #type; \
		typeDesc->size = sizeof(T); \
		typeDesc->members = {

#define REFLECT_STRUCT_MEMBER(name) \
			{#name, offsetof(T, name), Solus::TypeResolver<decltype(T::name)>::Get()},

#define REFLECT_STRUCT_END() \
		}; \
	}

	struct TypeDescriptor_StdVector : TypeDescriptor
	{
		TypeDescriptor* itemType;
		size_t(*GetSize)(const void*);
		const void* (*GetItem)(const void*, size_t);

		template<typename ItemType>
		TypeDescriptor_StdVector(ItemType*)
			: TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) },
			itemType{ TypeResolver<ItemType>::Get() }
		{
			GetSize = [](const void* vecPtr) -> size_t
			{
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return vec.size();
			};

			GetItem = [](const void* vecPtr, size_t index) -> const void*
			{
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return &vec[index];
			};
		}

		virtual std::string GetName() const override
		{
			return std::string("std::vector<") + itemType->GetName() + ">";
		}

		virtual void Print(const void* object, size_t indentLevel) const override
		{
			size_t numItems = GetSize(object);
			std::cout << GetName();
			if (numItems == 0)
			{
				std::cout << "{}";
			}
			else
			{
				std::cout << "{" << std::endl;
				for (size_t index = 0; index < numItems; index++)
				{
					std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
					itemType->Print(GetItem(object, index), indentLevel + 1);
					std::cout << std::endl;
				}
				std::cout << std::string(4 * indentLevel, ' ') << "}";
			}
		}
	};

	template<typename T>
	class TypeResolver<std::vector<T>>
	{
	public:
		static TypeDescriptor* Get()
		{
			static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};
	}

