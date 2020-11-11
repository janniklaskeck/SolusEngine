#include "FileUtils.h"
#include "Engine/Engine.h"

#include <filesystem>
#include <sstream>
#include <fstream>

namespace Solus
{
	using namespace std;

	std::string FileUtils::GetCurrentFolder()
	{
		
		fs::path path = fs::current_path();
		return path.string();
	}

	bool FileUtils::FileExists(fs::path path)
	{
		fs::file_status fileStatus = fs::status(path);
		return fs::exists(path) && fs::is_regular_file(fileStatus);
	}

	std::string FileUtils::ReadFile(fs::path path)
	{
		fs::file_status fileStatus = fs::status(path);
		if (!fs::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not load file at %s!", path.string().c_str());
			return "";
		}

		std::ifstream fileStream;
		fileStream.open(path);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		return std::string(ss.str());
	}

	char* FileUtils::ReadFileRaw(fs::path path, uintmax_t& length)
	{
		fs::file_status fileStatus = fs::status(path);
		if (!fs::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not load file at %s!", path.string().c_str());
			return nullptr;
		}
		uintmax_t fileSize = fs::file_size(path);
		length = fileSize;
		char* buffer = new char[fileSize];
		std::ifstream fileStream(path, std::ios::binary);
		fileStream.seekg(std::ios::beg);
		fileStream.read(buffer, fileSize);
		fileStream.close();
		return buffer;
	}

	bool FileUtils::CreateFile(const fs::path path)
	{
		if (FileExists(path))
			return false;
		std::ofstream fileStream(path);
		return true;
	}

	bool FileUtils::WriteToFile(fs::path path, const char* fileContent, bool append /*= false*/)
	{
		if (!FileExists(path))
			return false;

		auto mode = std::ios::out | std::ios::binary;
		if (append)
			mode |= std::ios::app;
		else
			mode |= std::ios::trunc;
		std::ofstream fileStream;
		fileStream.open(path, mode);
		fileStream << fileContent;

		return true;
	}
}
