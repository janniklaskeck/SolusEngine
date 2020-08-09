#include "ProjectFile.h"

#include "Utility/FileUtils.h"

#include <filesystem>

namespace Solus
{
	ProjectFile::ProjectFile(const std::string& filepath)
	{
		auto contents = FileUtils::ReadFile(filepath.c_str());
		projectData = nlohmann::json::parse(contents);

		auto rootPath = std::filesystem::path(filepath).parent_path() / "Assets";
		projectRootFolder = rootPath.string();
	}

	const std::string ProjectFile::GetProjectName() const
	{
		return projectData["projectName"];
	}

	const uint32_t ProjectFile::GetProjectVersion() const
	{
		return projectData["projectVersion"];
	}

	const std::string ProjectFile::GetProjectRootFolder() const
	{
		return projectRootFolder;
	}
}
