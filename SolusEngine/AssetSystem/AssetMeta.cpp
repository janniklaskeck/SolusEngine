#include "AssetMeta.h"
#include "Asset.h"
#include "Utility/FileUtils.h"
#include "Utility/Helper.h"

#include <filesystem>

namespace Solus
{
	AssetMeta::AssetMeta()
	{}

	void AssetMeta::Initialize(const Asset* asset)
	{
		std::filesystem::path filePath(asset->GetFilePath());
		std::filesystem::path parentFolder = filePath.parent_path();
		std::string fileName = filePath.stem().string();
		std::string fileExtension = filePath.extension().string();

		std::filesystem::path metaDataFileName(fileName + fileExtension + ASSET_FILE_EXTENSION);

		std::filesystem::path metaDataFilePath = parentFolder / metaDataFileName;
		std::string metaDataFilePathString = metaDataFilePath.string();
		if (std::filesystem::exists(metaDataFilePath))
		{
			ReadMetaData(metaDataFilePathString);
		}
		else
		{
			WriteMetaData(metaDataFilePathString);
			ReadMetaData(metaDataFilePathString);
		}
		if (!metaData.contains("id"))
		{
			metaData["id"] = GenerateUUID();
			WriteMetaData(metaDataFilePathString);
		}
	}

	void AssetMeta::ReadMetaData(const std::string& path)
	{
		auto contents = FileUtils::ReadFile(path.c_str());
		metaData = nlohmann::json::parse(contents);
	}

	uint32_t AssetMeta::GetMetaId() const
	{
		return metaData["id"];
	}

	void AssetMeta::WriteMetaData(const std::string& path)
	{
		if (metaData.is_null())
			metaData = nlohmann::json({});
		std::string content = metaData.dump();
		FileUtils::WriteFile(path.c_str(), content.c_str());
	}
}
