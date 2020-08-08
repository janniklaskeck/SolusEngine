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
		{
			reader = std::ifstream(path, std::ios::binary);
			if (!readBuffered)
			{
				readBuffered = true;
				reader.seekg(0, std::ios::end);
				const uint64_t size = reader.tellg();
				reader.seekg(0, std::ios::beg);
				char* _buffer = (char*)malloc(size);
				reader.read(_buffer, size);
				buffer = _buffer;
				reader.seekg(0, std::ios::beg);

				root.value = (unsigned char*)buffer;
				root.size = size;
			}
		}
		else if (_mode == ArchiveMode::WRITE)
			writer = std::ofstream(path, std::ios::binary);
	}

	void ArchiveStream::WriteString(const std::string& string)
	{
		uint32_t stringSize = (uint32_t)string.size();
		WriteUInt32((uint32_t)string.size());
		writer.write(string.c_str(), stringSize);
	}

	void ArchiveStream::WriteUInt32(const uint32_t value)
	{
		writer.write((char*)&value, sizeof(uint32_t));
	}

	void ArchiveStream::WriteUInt8(const uint8_t value)
	{
		writer.write((char*)&value, sizeof(uint8_t));
	}

	void ArchiveEntry::ReadString(std::string& out) const
	{
		const uint32_t size = ReadUInt32();
		out.resize(size);
		out.assign((char*)value + currentPosition, size);
		currentPosition += size;
	}

	uint8_t ArchiveEntry::ReadUInt8() const
	{
		return uint8_t(value[currentPosition++]);
	}

	uint32_t ArchiveEntry::ReadUInt32() const
	{
		unsigned char bytes[4] =
		{ 
			value[currentPosition++],
			value[currentPosition++],
			value[currentPosition++],
			value[currentPosition++]
		};
		return uint32_t((unsigned char)(bytes[3]) << 24 |
						(unsigned char)(bytes[2]) << 16 |
						(unsigned char)(bytes[1]) << 8 |
						(unsigned char)(bytes[0]));
	}
}
