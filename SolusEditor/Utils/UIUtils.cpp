#include "UIUtils.h"

namespace Editor
{
	namespace UIUtils
	{
		std::string ConvertSizeToString(uint64_t size)
		{
			std::string sizeType;
			if (size < KB_SIZE)
				sizeType = "bytes";
			else if (size >= KB_SIZE && size < MB_SIZE)
			{
				sizeType = "kb";
				size /= KB_SIZE;
			}
			else if (size >= MB_SIZE && size < GB_SIZE)
			{
				sizeType = "mb";
				size /= MB_SIZE;
			}
			else
			{
				sizeType = "gb";
				size /= GB_SIZE;
			}
			char sizeText[128];

			sprintf_s(sizeText, "%lld %s", size, sizeType.c_str());
			return sizeText;
		}
	}
}
