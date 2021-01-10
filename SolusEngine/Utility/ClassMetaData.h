#pragma once

#include "Engine/SolusEngine.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Solus
{
	class ArchiveStream;
	struct ArchiveEntry;
	class SObject;
	class SAsset;

	struct SOLUS_API ClassMetaData
	{
	public:
		virtual void Serialize(ArchiveStream* archive, const SObject* object) const
		{
		};

		void Deserialize(ArchiveStream* archive, ArchiveEntry& entry, SObject* object);

		virtual const bool DeserializeMember(ArchiveStream* archive, const SObject* object, ArchiveEntry& nextEntry)
		{
			return false;
		}

		template<typename T>
		T* GetMemberPtr(const SObject* object, const std::string& name)
		{
			void* ptr = GetMemberPtrInternal(object, name);
			return static_cast<T*>(ptr);
		}

		void SetMemberAsset(const SObject* object, const std::string& name, SAsset* value);

		void SetMemberAssetVector(const SObject* object, const std::string& name, const uint32_t index, SAsset* value);

	protected:
		virtual void* GetMemberPtrInternal(const SObject* object, const std::string& name)
		{
			return nullptr;
		}

	};
}
