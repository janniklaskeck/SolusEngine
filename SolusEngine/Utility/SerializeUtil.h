#pragma once

#include "Engine/Engine.h"

#include "Object/SObject.h"
#include "Object/Entity.h"
#include "Object/Component/SComponent.h"
#include "AssetSystem/Asset.h"

#include "Utility/Vector.h"

#include <rttr/registration.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

namespace Solus
{
	class SComponent;

	enum class ArchiveMode : uint8_t
	{
		READ,
		WRITE
	};

	enum class EntryType : uint8_t
	{
		None = 0,
		Object,
		Pointer,
		Map,
		Vector
	};

	struct ArchiveEntry
	{
		std::string name;
		std::string typeName;
		EntryType type = EntryType::None;
		uint64_t size = 0;
		unsigned char* value = nullptr;
		mutable uint32_t currentPosition = 0;

		void ReadString(std::string& out) const;
		uint32_t ReadUInt32() const;
		uint8_t ReadUInt8() const;
	};

	class SOLUS_API ArchiveStream
	{
	public:
		static const char BEGIN_BLOCK;
		static const char END_BLOCK;

		static const uint8_t OBJECT_TYPE_ID;
		static const uint8_t POINTER_TYPE_ID;
		static const uint8_t VECTOR_TYPE_ID;
		static const uint8_t MAP_TYPE_ID;

	private:
		ArchiveMode mode = ArchiveMode::READ;

		std::string path;
		std::ifstream reader;
		std::ofstream writer;

		bool readBuffered = false;
		char* buffer = nullptr;
	public:
		ArchiveEntry root;

	public:
		ArchiveStream(const std::string& path, ArchiveMode _mode = ArchiveMode::READ);

		ArchiveStream(const ArchiveStream&) = delete;
		ArchiveStream& operator=(const ArchiveStream&) = delete;

		~ArchiveStream()
		{
			if (buffer)
				free((void*)buffer);
		}
	public:

		bool Deserialize(ArchiveEntry& nextEntry, ArchiveEntry& parent);

		template<typename T>
		void Serialize(const std::string& name, const std::string& type, const T& value);

	private:

		template<typename T>
		std::enable_if_t<!std::is_pointer_v<T>> SerializeInternal(const std::string& name, const T& value);

		template<typename T>
		std::enable_if_t<std::is_trivial_v<T>> SerializeInternal(const std::string& name, const std::vector<T>& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> SerializeInternal(const std::string& name, const std::vector<T*>& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> SerializeInternal(const std::string& name, const T* value);

		// Only allow integral types as keys for now
		template <typename Key, typename Value>
		std::enable_if_t<std::is_integral_v<Key>> SerializeInternal(const std::string& name, const std::unordered_map<Key, Value>& map);

	public:

		template<typename T>
		std::enable_if_t<!std::is_pointer_v<T>> DeserializeInternal(ArchiveEntry& entry, T& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> DeserializeInternal(ArchiveEntry& entry, T* value);

		template <typename Key, typename Value>
		std::enable_if_t<std::is_integral_v<Key>> DeserializeInternal(ArchiveEntry& entry, std::unordered_map<Key, Value>& map);

		template<typename T>
		std::enable_if_t<std::is_trivial_v<T>> DeserializeInternal(ArchiveEntry& entry, std::vector<T>& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> DeserializeInternal(ArchiveEntry& entry, std::vector<T*>& value);

	private:

		void WriteString(const std::string& string);
		void WriteUInt32(const uint32_t value);
		void WriteUInt8(const uint8_t value);
	};

	///
	/// Serialize 
	///
	
	inline bool ArchiveStream::Deserialize(ArchiveEntry& outEntry, ArchiveEntry& parent)
	{
		if (parent.currentPosition >= parent.size)
			return false;

		parent.ReadString(outEntry.name);
		parent.ReadString(outEntry.typeName);
		outEntry.size = 0;
		outEntry.currentPosition = 0;
		
		parent.ReadUInt8(); // block begin
		outEntry.value = (unsigned char*)parent.value + parent.currentPosition;
		
		uint32_t stack = 1;
		while (stack > 0)
		{
			char nextChar = parent.value[parent.currentPosition++];
			if (nextChar == BEGIN_BLOCK)
				stack++;
			else if (nextChar == END_BLOCK)
				stack--;
			if (stack > 0)
				outEntry.size++;
		}
		
		return true;
	}

	template<typename T>
	inline void ArchiveStream::Serialize(const std::string& name, const std::string& type, const T& value)
	{
		WriteString(name);
		WriteString(type);

		WriteUInt8((uint8_t)BEGIN_BLOCK);
		SerializeInternal(name, value);
		WriteUInt8((uint8_t)END_BLOCK);
	}

	template<typename T>
	inline std::enable_if_t<!std::is_pointer_v<T>> ArchiveStream::SerializeInternal(const std::string& name, const T& value)
	{
		//WriteUInt8(OBJECT_TYPE_ID);
		WriteUInt32(sizeof(T));
		writer.write((char*)&value, sizeof(T));
	}

	template<typename T>
	std::enable_if_t<std::is_trivial_v<T>> ArchiveStream::SerializeInternal(const std::string& name, const std::vector<T>& value)
	{
		//WriteUInt8(VECTOR_TYPE_ID);
		WriteUInt32((uint32_t)value.size());

		for (size_t i = 0; i < value.size(); i++)
		{
			const std::string typeName = std::string(rttr::type::get<T>().get_name());
			Serialize(std::to_string(i), typeName, value[i]);
		}
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::SerializeInternal(const std::string& name, const std::vector<T*>& value)
	{
		//WriteUInt8(VECTOR_TYPE_ID);
		WriteUInt32((uint32_t)value.size());

		for (size_t i = 0; i < value.size(); i++)
		{
			const std::string typeName = std::string(value[i]->get_type().get_name());
			Serialize(std::to_string(i), typeName, value[i]);
		}
	}

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::SerializeInternal(const std::string& name, const T* value)
	{
		//WriteUInt8(POINTER_TYPE_ID);
		if (value)
			value->Serialize(*this);
	}

	template<typename Key, typename Value>
	std::enable_if_t<std::is_integral_v<Key>> ArchiveStream::SerializeInternal(const std::string& name, const std::unordered_map<Key, Value>& map)
	{
		//WriteUInt8(MAP_TYPE_ID);
		WriteUInt32((uint32_t)map.size());

		for (const auto& keyValue : map)
		{
			const auto key = keyValue.first;
			const auto value = keyValue.second;
			const std::string typeName = std::string(value->get_type().get_name());
			Serialize(std::to_string(key), typeName, value);
		}
	}

	///
	/// Deserialize 
	///

	template<typename T>
	inline std::enable_if_t<!std::is_pointer_v<T>> ArchiveStream::DeserializeInternal(ArchiveEntry& entry, T& value)
	{
		//entry.ReadUInt8(); // read type
		uint32_t size = entry.ReadUInt32();
		memcpy(&value, entry.value + entry.currentPosition, size);
	}

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::DeserializeInternal(ArchiveEntry& entry, T* value)
	{
		value->GetClassMetaData()->Deserialize(this, entry, value);
	}

	template<typename Key, typename Value>
	inline std::enable_if_t<std::is_integral_v<Key>> ArchiveStream::DeserializeInternal(ArchiveEntry& entry, std::unordered_map<Key, Value>& map)
	{
		//entry.ReadUInt8();
		uint32_t elements = entry.ReadUInt32();
		for (size_t i = 0; i < elements; i++)
		{
			ArchiveEntry newEntry;
			if (Deserialize(newEntry, entry))
			{
				rttr::variant keyValueString = newEntry.name;
				rttr::variant keyValue = keyValueString.convert<Key>();
				Key key = keyValue.get_value<Key>();
				Value newValue = rttr::type::get_by_name(newEntry.typeName).create().get_value<Value>();
				map[key] = newValue;
				DeserializeInternal(newEntry, newValue);
			}
		}
	}

	template<typename T>
	inline std::enable_if_t<std::is_trivial_v<T>> ArchiveStream::DeserializeInternal(ArchiveEntry& entry, std::vector<T>& value)
	{
		//entry.ReadUInt8();
		uint32_t elements = entry.ReadUInt32();
		for (size_t i = 0; i < elements; i++)
		{
			ArchiveEntry newEntry;
			if (Deserialize(newEntry, entry))
			{
				value[i] = rttr::type::get_by_name(newEntry.typeName).create().get_value<T*>();
				DeserializeInternal(newEntry, value[i]);
			}
		}
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<SObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::DeserializeInternal(ArchiveEntry& entry, std::vector<T*>& value)
	{
		//entry.ReadUInt8();
		uint32_t elements = entry.ReadUInt32();
		value.resize(elements);
		for (size_t i = 0; i < elements; i++)
		{
			ArchiveEntry newEntry;
			if (Deserialize(newEntry, entry))
			{
				value[i] = rttr::type::get_by_name(newEntry.typeName).create().get_value<T*>();
				DeserializeInternal(newEntry, value[i]);
			}
		}
	}

}
