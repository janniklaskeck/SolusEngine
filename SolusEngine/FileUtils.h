#pragma once

#include "SolusEngine.h"

#include <string>
#include <cstdint>

namespace Solus
{

	class SOLUS_API FileUtils
	{
	private:
		FileUtils(){}

	public:
		
		static std::string GetCurrentFolder();

		static std::string ReadFile(const char* filePath);
		static char* ReadFileRaw(const char* filePath, uintmax_t& length);
		static bool WriteFile(const char* filePath, const char* fileContent);


	};
}

