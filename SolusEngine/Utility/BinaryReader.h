#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class Asset;

	class SOLUS_API BinaryReader
	{
	public:
		BinaryReader(Asset* asset);

		void ReadBytes(unsigned char* dst, uint32_t amount);

		int32_t ReadInt32();
		float ReadFloat();
		double ReadDouble();

		bool ReadBool();

		char ReadChar();

		std::string ReadString(uint32_t length);

	private:
		void IncreaseOffset(uint32_t amount);

		Asset* asset;
		void* data;
		uint32_t currentOffset = 0;
		uint32_t dataLength = 0;
	};

}
