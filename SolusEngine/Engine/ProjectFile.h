#pragma once

#include "SolusEngine.h"

#include "nlohmann/json.hpp"

namespace Solus
{
	class SOLUS_API ProjectFile
	{
	public:
		ProjectFile(const std::string& filepath);

		const std::string GetProjectName() const;
		const uint32_t GetProjectVersion() const;
		const std::string GetProjectRootFolder() const;

	private:
		nlohmann::json projectData;
		std::string projectRootFolder;
	};

}
