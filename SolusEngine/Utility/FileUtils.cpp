#include "FileUtils.h"
#include "Engine/Engine.h"

#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>

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

	bool FileUtils::FolderExists(fs::path path)
	{
		fs::file_status fileStatus = fs::status(path);
		return fs::exists(path) && fs::is_directory(fileStatus);
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

	bool FileUtils::ReadFileRaw(fs::path path, std::vector<unsigned char>& bytes)
	{
		fs::file_status fileStatus = fs::status(path);
		if (!fs::is_regular_file(fileStatus))
		{
			gEngine->Log(LogLevel::LogError, "Could not load file at %s!", path.string().c_str());
			return false;
		}

		std::streampos fileSize;
		std::ifstream file(path, std::ios::binary);

		// get its size:
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// read the data:
		bytes.resize(fileSize);
		file.read((char*)&bytes[0], fileSize);
		std::stringstream ss;
		for (int i = 0; i < fileSize; i++)
		{
			unsigned int byte = bytes.data()[i];
			ss << std::hex << std::setfill('0') << std::setw(2) << byte << std::endl;
		}
		printf("%s", ss.str().c_str());
		return true;
	}

	bool FileUtils::CreateFile(const fs::path path)
	{
		if (FileExists(path))
			return false;
		std::ofstream fileStream(path);
		return fileStream.good();
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
