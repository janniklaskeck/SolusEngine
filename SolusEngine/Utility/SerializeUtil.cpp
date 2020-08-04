#include "SerializeUtil.h"

#include <rttr/registration.h>

namespace Solus
{
	const char ArchiveStream::BEGIN_BLOCK = '{';
	const char ArchiveStream::END_BLOCK = '}';

	const uint8_t ArchiveStream::OBJECT_TYPE_ID = 0;
	const uint8_t ArchiveStream::POINTER_TYPE_ID = 1;
	const uint8_t ArchiveStream::VECTOR_TYPE_ID = 2;
	const uint8_t ArchiveStream::MAP_TYPE_ID = 3;

	ArchiveStream::ArchiveStream(const std::string& _path, ArchiveMode _mode /*= ArchiveMode::READ*/)
		: mode(_mode),
		path(_path)
	{
		if (_mode == ArchiveMode::READ)
			reader = std::ifstream(path, std::ios::binary);
		else if(_mode == ArchiveMode::WRITE)
			writer = std::ofstream(path, std::ios::binary);
	}

	void ArchiveStream::WriteString(const std::string& string)
	{
		uint32_t stringSize = (uint32_t)string.size();
		WriteUInt32((uint32_t)string.size());
		writer.write(string.c_str(), stringSize);
	}

	void ArchiveStream::ReadString(std::string& out)
	{
		const uint32_t size = ReadUInt32();
		out.resize(size);
		reader.read(out.data(), size);
	}

	void ArchiveStream::WriteUInt32(const uint32_t value)
	{
		writer.write((char*)&value, sizeof(uint32_t));
	}

	void ArchiveStream::WriteUInt8(const uint8_t value)
	{
		writer.write((char*)&value, sizeof(uint8_t));
	}

	uint8_t ArchiveStream::ReadUInt8()
	{
		char value[1];
		reader.read(value, 1);
		return uint8_t(value[0]);
	}

	uint32_t ArchiveStream::ReadUInt32()
	{
		char value[4];
		reader.read(value, 4);
		return uint32_t((unsigned char)(value[3]) << 24 |
						(unsigned char)(value[2]) << 16 |
						(unsigned char)(value[1]) << 8 |
						(unsigned char)(value[0]));
	}

	uint32_t ArchiveEntry::Read(const unsigned char* ptr)
	{
		uint32_t offset = 0;
		name = ReadString(ptr);
		offset += sizeof(uint32_t) + (uint32_t)name.size();

		typeName = ReadString(ptr + offset);
		type = GetType(typeName);
		offset += sizeof(uint32_t) + (uint32_t)typeName.size();

		if (type == EntryType::Map || type == EntryType::Vector)
		{
			uint32_t elementCount = ReadUInt(ptr + offset);
			offset += sizeof(uint32_t);
			for (uint32_t i = 0; i < elementCount; i++)
			{
				ArchiveEntry subEntry;
				uint32_t subEntrySize = subEntry.Read(ptr + offset);
				offset += subEntrySize;
				subEntry.parentEntry = this;
				subEntries.push_back(subEntry);
			}
		}
		else if (type == EntryType::Pointer)
		{
			uint32_t stackSize = 0;
			while (true)
			{
				const std::string token = ReadString(ptr + offset);
				if (token[0] == ArchiveStream::BEGIN_BLOCK)
				{
					++stackSize;
					offset += sizeof(uint32_t) + (uint32_t)token.size();
				}
				else if (token[0] == ArchiveStream::END_BLOCK)
				{
					--stackSize;
					offset += sizeof(uint32_t) + (uint32_t)token.size();
				}

				if (stackSize > 0)
				{
					ArchiveEntry subEntry;
					uint32_t subEntrySize = subEntry.Read(ptr + offset);
					offset += subEntrySize;
					subEntries.push_back(subEntry);
				}
				else
					break;
			}
		}
		else
		{
			size = ReadUInt(ptr + offset);
			offset += 4;
			value = (ptr + offset);
		}

		offset += size;
		return offset;
	}

	void ArchiveEntry::Rebuild(SolusObject* root) const
	{
		const auto& rootType = root->get_type();

		switch (type)
		{
		case EntryType::Object:
		{
			if (name == "assetId")
			{
				bool isAsset = false;
				auto baseClasses = rttr::type::get_by_name(parentEntry->typeName).get_base_classes();
				for (auto& baseClass : baseClasses)
				{
					if (baseClass == rttr::type::get<Asset>())
					{
						isAsset = true;
						break;
					}
				}
				if (isAsset)
				{

					break;
				}
			}
			ClassMetaData* metaDataPtr = root->GetClassMetaData();
			if (metaDataPtr)
				metaDataPtr->DeserializeMember(root, name, value, size);

			break;
		}
		case EntryType::Pointer:
		{
			for (const auto& entry : subEntries)
			{
				entry.Rebuild(root);
			}
			break;
		}
		case EntryType::Map:
		{
			const auto& prop = rootType.get_property(name);
			if (prop.is_valid())
			{
				const auto& propValue = prop.get_value(root);
				if (propValue.is_valid())
				{
					auto variant = propValue.get_value<std::unordered_map<uint64_t, Entity*>>();
					std::unordered_map<uint64_t, Entity*>& value = variant;
					for (const auto& entry : subEntries)
					{
						uint64_t key = std::stoi(entry.name);
						const std::string realTypeName = entry.typeName.substr(0, entry.typeName.size() - 1);
						auto objectType = rttr::type::get_by_name(realTypeName);
						auto created = objectType.create();
						if (created.is_valid())
						{
							Entity* object = created.get_value<Entity*>();
							entry.Rebuild((SolusObject*)object);
							value.insert(std::make_pair(key, object));
						}
					}
					prop.set_value(root, value);
				}
			}
			break;
		}
		case EntryType::Vector:
		{
			const auto& prop = rootType.get_property(name);
			if (prop.is_valid())
			{
				const auto& propValue = prop.get_value(root);
				if (propValue.is_valid())
				{

					for (const auto& entry : subEntries)
					{
						const std::string realTypeName = entry.typeName.substr(0, entry.typeName.size() - 1);
						auto objectType = rttr::type::get_by_name(realTypeName);

					}
					prop.set_value(root, value);
				}
			}
			break;
		}
		default:
			break;
		}
	}

	const EntryType ArchiveEntry::GetType(const std::string& typeString) const
	{
		if (typeString.empty())
			return EntryType::None;
		if (typeString.back() == '*')
			return EntryType::Pointer;
		if (typeString.find("std::unordered_map") != std::string::npos)
			return EntryType::Map;
		if (typeString.find("std::vector") != std::string::npos)
			return EntryType::Vector;

		return EntryType::Object;
	}
}
