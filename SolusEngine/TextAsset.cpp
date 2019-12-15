#include "TextAsset.h"

namespace Solus
{
	std::string TextAsset::GetStringContent() const
	{
		std::string content = (char*)GetRawData();
		content[GetDataSize()] = '\0';
		return std::string(content);
	}

	const char* TextAsset::GetCharContent() const
	{
		char* data = (char*)GetRawData();
		data[dataLength] = '\0';
		return data;
	}
}
