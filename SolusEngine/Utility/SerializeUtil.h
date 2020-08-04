#pragma once

#include "Engine/Engine.h"

#include "Object/SolusObject.h"
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
		uint32_t size = 0;
		const unsigned char* value = nullptr;

		ArchiveEntry* parentEntry;
		std::vector<ArchiveEntry> subEntries;

		uint32_t Read(const unsigned char* ptr);

		void Rebuild(SolusObject* root) const;

	private:

		const EntryType GetType(const std::string& typeString) const;

		uint32_t ReadUInt(const unsigned char* ptr)
		{
			return uint32_t((unsigned char)(ptr[3]) << 24 |
							(unsigned char)(ptr[2]) << 16 |
							(unsigned char)(ptr[1]) << 8 |
							(unsigned char)(ptr[0]));
		}

		std::string ReadString(const unsigned char* ptr)
		{
			uint32_t size = ReadUInt(ptr);
			return std::string((const char*)(ptr + sizeof(uint32_t)), size);
		}
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
		ArchiveEntry root;

	public:
		ArchiveStream(const std::string& path, ArchiveMode _mode = ArchiveMode::READ);

		ArchiveStream(const ArchiveStream&) = delete;
		ArchiveStream& operator=(const ArchiveStream&) = delete;

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> Deserialize1(T* object);

		template<typename T>
		void Deserialize(std::string& name, std::string& type, T& value);

		template<typename T>
		void Serialize(const std::string& name, const std::string& type, const T& value);

	private:

		template<typename T>
		std::enable_if_t<!std::is_pointer_v<T>> SerializeInternal(const std::string& name, const T& value);

		template<typename T>
		void SerializeInternal(const std::string& name, const std::vector<T>& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> SerializeInternal(const std::string& name, const T* value);

		// Only allow integral types as keys for now
		template <typename Key, typename Value>
		std::enable_if_t<std::is_integral_v<Key>> SerializeInternal(const std::string& name, const std::unordered_map<Key, Value>& map);

	private:
		void ParseFile()
		{
			std::string nextName;
			ReadString(nextName);
			std::string nextType;
			ReadString(nextType);

			char blockStart = ReadUInt8();
		}

		template<typename T>
		std::enable_if_t<!std::is_pointer_v<T>> DeserializeInternal(T& value);

		template<typename T>
		std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> DeserializeInternal(T* value);

		template <typename Key, typename Value>
		std::enable_if_t<std::is_integral_v<Key>> DeserializeInternal(std::unordered_map<Key, Value>& map);

		template<typename T>
		void DeserializeInternal(std::vector<T>& value);

	private:

		void WriteString(const std::string& string);

		void ReadString(std::string& out);

		void WriteUInt32(const uint32_t value);

		uint32_t ReadUInt32();

		void WriteUInt8(const uint8_t value);

		uint8_t ReadUInt8();
	};

	///
	/// Serialize 
	///

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::Deserialize1(T* object)
	{
		ArchiveEntry root;
		if (reader)
		{
			reader.seekg(0, std::ios::end);
			const uint64_t size = reader.tellg();
			reader.seekg(0, std::ios::beg);
			char* buffer = (char*)malloc(size);
			reader.read(buffer, size);

			root.Read((const unsigned char*)(buffer));
			root.Rebuild(object);

			free(buffer);
		}
	}

	template<typename T>
	inline void ArchiveStream::Deserialize(std::string& name, std::string& type, T& value)
	{
		ReadString(name);
		ReadString(type);

		DeserializeInternal(value);
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
		WriteUInt8(OBJECT_TYPE_ID);
		WriteUInt32(sizeof(T));
		writer.write((char*)&value, sizeof(T));
	}

	template<typename T>
	void ArchiveStream::SerializeInternal(const std::string& name, const std::vector<T>& value)
	{
		WriteUInt8(VECTOR_TYPE_ID);
		WriteUInt32((uint32_t)value.size());

		for (size_t i = 0; i < value.size(); i++)
		{
			SerializeInternal(std::to_string(i), value[i]);
		}
	}

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::SerializeInternal(const std::string& name, const T* value)
	{
		WriteUInt8(POINTER_TYPE_ID);
		if (value)
			value->Serialize(*this);
	}

	template<typename Key, typename Value>
	std::enable_if_t<std::is_integral_v<Key>> ArchiveStream::SerializeInternal(const std::string& name, const std::unordered_map<Key, Value>& map)
	{
		WriteUInt8(MAP_TYPE_ID);
		WriteUInt32((uint32_t)map.size());

		for (const auto& keyValue : map)
		{
			const auto key = keyValue.first;
			const auto value = keyValue.second;
			const std::string typeName = std::string(rttr::type::get<Value>().get_name());
			Serialize(std::to_string(key), typeName, value);
		}
	}

	///
	/// Deserialize 
	///

	template<typename T>
	inline std::enable_if_t<!std::is_pointer_v<T>> ArchiveStream::DeserializeInternal(T& value)
	{
		
	}

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<SolusObject, T> || std::is_base_of_v<SComponent, T> || std::is_base_of_v<Asset, T>> ArchiveStream::DeserializeInternal(T* value)
	{
	}

	template<typename Key, typename Value>
	inline std::enable_if_t<std::is_integral_v<Key>> ArchiveStream::DeserializeInternal(std::unordered_map<Key, Value>& map)
	{
	}

	template<typename T>
	inline void ArchiveStream::DeserializeInternal(std::vector<T>& value)
	{
		const int count = 0;
		for (size_t i = 0; i < count; i++)
		{

		}
	}

}
