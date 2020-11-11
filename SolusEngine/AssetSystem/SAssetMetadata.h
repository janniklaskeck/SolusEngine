#pragma once

#include "SAssetType.h"
#include "Utility/FileUtils.h"

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>


namespace Solus
{
	using json = nlohmann::json;

	class SOLUS_API SAssetMetadata
	{
	public:
		SAssetMetadata(const fs::path _filePath)
			: filePath(_filePath)
		{
			jsonData = nlohmann::json::parse(FileUtils::ReadFile(filePath));
		}

		static bool Create(const fs::path filePath, const SAssetType assetType);
	
		const SUUID GetAssetID() const
		{
			auto idString = jsonData["id"].get<std::string>();
			return SUUID::FromBytes(idString.c_str());
		}

		SAssetType GetAssetType() const
		{
			return (SAssetType)jsonData["type"].get<int>();
		}

		void SetSourceFilePath(const fs::path sourceFilePath)
		{
			jsonData["sourceFilePath"] = sourceFilePath;
		}

		const fs::path GetSourceFilePath() const
		{
			return jsonData["sourceFilePath"].get<std::string>();
		}

		void SetDataField(const std::string& name, const std::string& value)
		{
			jsonData["data"][name] = value;
		}

		const std::string GetDataField(const std::string& name) const
		{
			return jsonData["data"][name].get<std::string>();
		}

	private:
		json jsonData;
		fs::path filePath;
	};

}
