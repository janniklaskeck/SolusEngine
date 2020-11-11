#include "BinaryReader.h"
#include "AssetSystem/Asset.h"

namespace Solus
{

	BinaryReader::BinaryReader(void* data, uint32_t length)
	{
		this->data = (unsigned char*)data;
		this->dataLength = length;
	}

	void BinaryReader::ReadBytes(unsigned char* dst, uint32_t amount)
	{
		memcpy(dst, &data[currentOffset], amount);
		IncreaseOffset(amount);
	}

	int16_t BinaryReader::ReadInt16()
	{
		unsigned char* intData = data + currentOffset;
		unsigned char intBytes[sizeof(int16_t)] = { intData[0], intData[1] };
		int16_t value;
		memcpy(&value, &intBytes, sizeof(int16_t));
		IncreaseOffset(sizeof(int16_t));
		return value;
	}

	int32_t BinaryReader::ReadInt32()
	{
		unsigned char* intData = data + currentOffset;
		unsigned char intBytes[sizeof(int32_t)] = { intData[0], intData[1], intData[2], intData[3] };
		int32_t value;
		memcpy(&value, &intBytes, sizeof(int32_t));
		IncreaseOffset(sizeof(int32_t));
		return value;
	}

	float BinaryReader::ReadFloat()
	{
		unsigned char* intData = data + currentOffset;
		unsigned char intBytes[sizeof(int32_t)] = { intData[0], intData[1], intData[2], intData[3] };
		float value;
		memcpy(&value, &intBytes, sizeof(float));
		IncreaseOffset(sizeof(float));
		return value;
	}

	double BinaryReader::ReadDouble()
	{
		unsigned char* doubleData = data + currentOffset;
		unsigned char doubleBytes[sizeof(double)] = { doubleData[0], doubleData[1], doubleData[2], doubleData[3], doubleData[4], doubleData[5], doubleData[6], doubleData[7] };
		double value;
		memcpy(&value, &doubleBytes, sizeof(double));
		IncreaseOffset(sizeof(double));
		return value;
	}

	bool BinaryReader::ReadBool()
	{
		auto c = ReadChar();
		return c != 0;
	}

	char BinaryReader::ReadChar()
	{
		char c = data[currentOffset];
		IncreaseOffset(1);
		return c;
	}

	std::string BinaryReader::ReadString(uint32_t length)
	{
		unsigned char* charData = data + currentOffset;
		std::string string;
		string.resize(length);
		for (uint32_t i = 0; i < length; i++)
		{
			string[i] = charData[i];
		}
		IncreaseOffset(length);
		return string;
	}

	void BinaryReader::IncreaseOffset(uint32_t amount)
	{
		if (currentOffset + amount <= dataLength)
			currentOffset += amount;
	}

}
