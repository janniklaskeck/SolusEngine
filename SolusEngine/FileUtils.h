#pragma once

#include "SolusEngine.h"

#include <string>

namespace Solus
{

	class SOLUS_API FileUtils
	{
	private:
		FileUtils(){}

	public:
		
		static std::string GetCurrentFolder();

		static std::string ReadFile(const char* filePath);
	};
}

