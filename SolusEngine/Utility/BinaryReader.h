#pragma once

#include "Engine/SolusEngine.h"

#include "FileUtils.h"

namespace Solus
{

	class SOLUS_API BinaryReader
	{
	public:
		BinaryReader(const void* data, uint32_t length);

		void ReadBytes(unsigned char* dst, uint32_t amount);

		int16_t ReadInt16();
		int32_t ReadInt32();
		float ReadFloat();
		double ReadDouble();

		bool ReadBool();

		char ReadChar();

		std::string ReadString(uint32_t length);

	private:
		void IncreaseOffset(uint32_t amount);

		fs::path path;
		const unsigned char* data;
		uint64_t currentOffset = 0;
		uint64_t dataLength = 0;
	};

}
