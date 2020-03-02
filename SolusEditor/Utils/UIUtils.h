#pragma once

#include <string>
#include <cstdint>

namespace Editor
{
	
	namespace UIUtils
	{
		const uint64_t KB_SIZE = 1024;
		const uint64_t MB_SIZE = KB_SIZE * KB_SIZE;
		const uint64_t GB_SIZE = MB_SIZE * MB_SIZE;

		std::string ConvertSizeToString(uint64_t size);

	}
}