#include "AssetMeta.h"
#include "FileUtils.h"
#include "Asset.h"

#include <filesystem>

namespace Solus
{
	AssetMeta::AssetMeta()
	{}

	void AssetMeta::Initialize(Asset* asset)
	{
		std::filesystem::path filePath(asset->GetFilePath());
		std::filesystem::path parentFolder = filePath.parent_path();
		std::string fileName = filePath.stem().string();
		std::string fileExtension = filePath.extension().string();

		std::filesystem::path metaDataFileName(fileName + fileExtension + Asset::ASSET_FILE_EXTENSION);

		std::filesystem::path metaDataFilePath = parentFolder / metaDataFileName;
		std::string metaDataFilePathString = metaDataFilePath.string();
		if (std::filesystem::exists(metaDataFilePath))
		{
			ReadMetaData(metaDataFilePathString);
		}
		else
		{
			CreateMetaData(metaDataFilePathString);
			ReadMetaData(metaDataFilePathString);
		}
	}

	void AssetMeta::ReadMetaData(std::string& path)
	{
		metaData = nlohmann::json::parse(FileUtils::ReadFile(path.c_str()).c_str());
	}

	void AssetMeta::CreateMetaData(std::string& path)
	{
		FileUtils::WriteFile(path.c_str(), metaData.dump().c_str());
	}
}
