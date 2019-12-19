#pragma once

#include "Engine/SolusEngine.h"

#include <string>
#include <cstdint>
#include <filesystem>

namespace Solus
{

	class SOLUS_API FileUtils
	{
	private:
		FileUtils(){}

	public:
		
		static std::string GetCurrentFolder();

		static std::string ReadFile(std::filesystem::path path);
		static char* ReadFileRaw(std::filesystem::path path, uintmax_t& length);
		static bool WriteFile(const char* filePath, const char* fileContent);


	};
}

