#pragma once

#include "Engine/SolusEngine.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Solus
{
	struct SOLUS_API ClassMetaData
	{
	public:
		struct TypeInfo
		{
			const char* name = "";
			size_t size = 0;

			bool IsType(const char* typeName)
			{
				return _stricmp(name, typeName) == 0;
			}

			virtual void* GetValuePtr(void* object) = 0;
		};

		size_t id;
		const char* name;
		size_t size;

		std::vector<ClassMetaData*> parents;
		
		std::unordered_map<std::string, TypeInfo*> data;
		std::vector<std::string> sortedMemberKeys;

		template<typename V>
		static V* GetValuePtr(const char* name, void* object);
		static std::vector<ClassMetaData*> classMetaData;
		static void Insert(size_t id, ClassMetaData* metaData);
		static ClassMetaData* Get(size_t id);
	private:
		// Used only to cast the void* object to SolusObject to access the class id hash
		static ClassMetaData* GetClassMetaData(void* object);
		static TypeInfo* GetTypeInfo(const char* name, ClassMetaData* metaData);
	};

	template<typename V>
	inline V* ClassMetaData::GetValuePtr(const char* name, void* object)
	{
		ClassMetaData* typeData = GetClassMetaData(object);
		if (!typeData)
			return nullptr;
		TypeInfo* typeInfo = GetTypeInfo(name, typeData);
		if (!typeInfo)
			return nullptr;
		return (V*)typeInfo->GetValuePtr(object);
	}

}
