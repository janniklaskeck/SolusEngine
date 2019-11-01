#include "FileUtils.h"
#include "Engine.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <sstream>

namespace Solus
{
	using namespace boost;

	std::string FileUtils::GetCurrentFolder()
	{
		filesystem::path path = filesystem::current_path();
		return path.string();
	}
	std::string FileUtils::ReadFile(const char* filePath)
	{
		filesystem::path _filePath;
		_filePath /= GetCurrentFolder();
		_filePath /= filePath;

		filesystem::file_status fileStatus = filesystem::status(_filePath);
		if (!filesystem::is_regular_file(fileStatus))
		{
			Engine::Log(LogLevel::LogError, "Could not load file at %s!", _filePath.string().c_str());
			return "";
		}

		filesystem::ifstream fileStream;
		fileStream.open(_filePath);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		return ss.str();
	}
}
