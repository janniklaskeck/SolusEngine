#include "BinaryReader.h"
#include "AssetSystem/Asset.h"

Solus::BinaryReader::BinaryReader(Asset* asset)
	: asset(asset), currentOffset(0)
{
	data = asset->GetRawData();
	dataLength = asset->GetDataSize();
}

void Solus::BinaryReader::ReadBytes(unsigned char* dst, uint32_t amount)
{
	unsigned char* charData = ((unsigned char*)(data));
	memcpy(dst, &charData[currentOffset], amount);
	IncreaseOffset(amount);
}

int32_t Solus::BinaryReader::ReadInt32()
{
	unsigned char* intData = ((unsigned char*)(data));
	intData += currentOffset;
	unsigned char intBytes[sizeof(int32_t)] = { intData[3], intData[2], intData[1], intData[0] };
	int32_t value;
	memcpy(&value, &intBytes, sizeof(int32_t));
	IncreaseOffset(sizeof(int32_t));
	return value;
}

float Solus::BinaryReader::ReadFloat()
{
	unsigned char* intData = ((unsigned char*)(data));
	intData += currentOffset;
	unsigned char intBytes[sizeof(float)] = { intData[3], intData[2], intData[1], intData[0] };
	float value;
	memcpy(&value, &intBytes, sizeof(float));
	IncreaseOffset(sizeof(float));
	return value;
}

double Solus::BinaryReader::ReadDouble()
{
	return 0.0;
}

bool Solus::BinaryReader::ReadBool()
{
	return false;
}

char Solus::BinaryReader::ReadChar()
{
	return 0;
}

std::string Solus::BinaryReader::ReadString(uint32_t length)
{
	char* charData = (char*)data;
	charData += currentOffset;
	std::string string = "";
	for (uint32_t i = 0; i < length; i++)
	{
		string += charData[i];
	}
	string[length] = '\0';
	currentOffset += length;
	return string;
}

void Solus::BinaryReader::IncreaseOffset(uint32_t amount)
{
	if (currentOffset + amount <= dataLength)
		currentOffset += amount;
}
