#include "AssetMeta.h"
#include "FileUtils.h"
#include "Asset.h"

#include <boost/filesystem.hpp>

using namespace boost;

namespace Solus
{
	AssetMeta::AssetMeta()
	{
		metaData = nlohmann::json::parse("{}");
	}

	void AssetMeta::Initialize(Asset* asset)
	{
		filesystem::path filePath(asset->GetFilePath());
		filesystem::path parentFolder = filePath.parent_path();
		std::string fileName = filePath.stem().string();
		std::string fileExtension = filePath.extension().string();

		filesystem::path metaDataFileName(fileName + fileExtension + Asset::ASSET_FILE_EXTENSION);

		filesystem::path metaDataFilePath = parentFolder / metaDataFileName;
		std::string metaDataFilePathString = metaDataFilePath.relative_path().string();
		if (filesystem::exists(metaDataFilePath))
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
