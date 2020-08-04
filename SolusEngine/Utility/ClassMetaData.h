#pragma once

#include "Engine/SolusEngine.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Solus
{
	class ArchiveStream;
	class SolusObject;

	struct SOLUS_API ClassMetaData
	{
	public:
		virtual void Serialize(ArchiveStream* archive, const SolusObject* object) const
		{
		};

		virtual void Deserialize(ArchiveStream* archive, SolusObject* object)
		{};

		virtual const bool DeserializeMember(const SolusObject* object, const std::string& name, const unsigned char* ptr, uint32_t length)
		{
			return false;
		}

		template<typename T>
		T* GetMemberPtr(const SolusObject* object, const std::string& name)
		{
			void* ptr = GetMemberPtrInternal(object, name);
			return static_cast<T*>(ptr);
		}

	protected:
		virtual void* GetMemberPtrInternal(const SolusObject* object, const std::string& name)
		{
			return nullptr;
		}

	};
}
