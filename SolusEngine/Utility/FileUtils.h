#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class SOLUS_API FileUtils
	{
	private:
		FileUtils() = default;

	public:
		
		static std::string GetCurrentFolder();

		static bool FileExists(fs::path path);

		static std::string ReadFile(fs::path path);
		static char* ReadFileRaw(fs::path path, uintmax_t& length);

		static bool CreateFile(const fs::path path);
		static bool WriteToFile(fs::path path, const char* fileContent, bool append = false);
	};
}

