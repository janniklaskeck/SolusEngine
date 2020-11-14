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
		SAssetMetadata(const fs::path _filePath);

		static bool Create(const fs::path filePath, const SAssetType assetType);
	
		const SUUID GetAssetID() const;

		SAssetType GetAssetType() const;

		void SetSourceFilePath(const fs::path sourceFilePath);

		const fs::path GetSourceFilePath() const;

		void SetDataField(const std::string& name, const std::string& value);

		const std::string GetDataField(const std::string& name) const;

	private:
		json jsonData;
		fs::path filePath;
	};

}
