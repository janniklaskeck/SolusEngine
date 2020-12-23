#pragma once

#include "SAssetMetadata.h"

namespace Solus
{

	SAssetMetadata::SAssetMetadata(const fs::path _filePath) 
		: filePath(_filePath)
	{
		jsonData = nlohmann::json::parse(FileUtils::ReadFile(filePath));
	}

	bool SAssetMetadata::Create(const fs::path filePath, const SAssetType assetType)
	{
		if (!FileUtils::CreateFile(filePath))
		{
			return false;
		}
		json newData;
		SUUID assetId;
		const std::string assetIdString = assetId.ToString();
		newData["id"] = assetIdString;
		newData["type"] = (int)assetType;
		newData["data"] = json::object();
		const std::string jsonString = newData.dump();
		FileUtils::WriteToFile(filePath, jsonString.c_str());
		return true;
	}

	const Solus::SUUID SAssetMetadata::GetAssetID() const
	{
		auto idString = jsonData["id"].get<std::string>();
		return SUUID::FromString(idString);
	}

	Solus::SAssetType SAssetMetadata::GetAssetType() const
	{
		return (SAssetType)jsonData["type"].get<int>();
	}

	void SAssetMetadata::SetSourceFilePath(const fs::path sourceFilePath)
	{
		jsonData["sourceFilePath"] = sourceFilePath.string();
	}

	const fs::path SAssetMetadata::GetSourceFilePath() const
	{
		return jsonData["sourceFilePath"].get<std::string>();
	}

	void SAssetMetadata::SetDataField(const std::string& name, const std::string& value)
	{
		jsonData["data"][name] = value;
	}

	const std::string SAssetMetadata::GetDataField(const std::string& name) const
	{
		return jsonData["data"][name].get<std::string>();
	}

	bool SAssetMetadata::SaveMetaData() const
	{
		return FileUtils::WriteToFile(filePath, jsonData.dump().c_str());
	}

}
