#include "FileUtils.h"
#include "Engine.h"

#include <filesystem>
#include <sstream>
#include <fstream>

namespace Solus
{
	using namespace std;

	std::string FileUtils::GetCurrentFolder()
	{
		
		filesystem::path path = filesystem::current_path();
		return path.string();
	}

	std::string FileUtils::ReadFile(std::filesystem::path path)
	{
		filesystem::file_status fileStatus = filesystem::status(path);
		if (!filesystem::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not load file at %s!", path.string().c_str());
			return "";
		}

		std::ifstream fileStream;
		fileStream.open(path);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		return ss.str();
	}

	char* FileUtils::ReadFileRaw(std::filesystem::path path, uintmax_t& length)
	{
		filesystem::file_status fileStatus = filesystem::status(path);
		if (!filesystem::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not load file at %s!", path.string().c_str());
			return nullptr;
		}
		uintmax_t fileSize = filesystem::file_size(path);
		length = fileSize;
		char* buffer = new char[fileSize];
		std::ifstream fileStream(path, std::ios::binary);
		fileStream.seekg(std::ios::beg);
		fileStream.read(buffer, fileSize);
		fileStream.close();
		return buffer;
	}

	bool FileUtils::WriteFile(const char* filePath, const char* fileContent)
	{
		filesystem::path _filePath(filePath);

		filesystem::file_status fileStatus = filesystem::status(_filePath);
		if (filesystem::exists(_filePath) && !filesystem::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not write file at %s!", _filePath.string().c_str());
			return false;
		}

		std::ofstream fileStream{ _filePath };
		fileStream << fileContent;


		return true;
	}
}
