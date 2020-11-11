#include "SAsset.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

#include "ShaderAsset.h"

namespace Solus
{
	const std::string SAsset::ASSET_EXT = ".asset";

	SAsset::SAsset(const fs::path filePath)
	{
		assetFilePath = filePath;
		if (FileUtils::FileExists(assetFilePath))
		{
			LoadMetadata();
		}
		else
		{
			assert(false);
		}
	}

	SAsset::SAsset(const fs::path filePath, const SAssetType assetType)
	{
		assetFilePath = filePath.string() + ASSET_EXT;
		if (FileUtils::FileExists(assetFilePath))
		{
			assert(false);
		}
		else
		{
			SAssetMetadata::Create(assetFilePath, assetType);
			LoadMetadata();
			metaData->SetSourceFilePath(filePath);
		}
	}

	bool SAsset::LoadMetadata()
	{
		metaData = std::make_unique<SAssetMetadata>(assetFilePath);
		return true;
	}

	const fs::path SAsset::GetAssetFilePath() const
	{
		return assetFilePath;
	}

	const fs::path SAsset::GetSourceFilePath() const
	{
		return metaData->GetSourceFilePath();
	}

	const std::string SAsset::GetFileName() const
	{
		return metaData->GetSourceFilePath().string();
	}

	const SAssetType SAsset::GetAssetType() const
	{
		return metaData->GetAssetType();
	}

	const SUUID SAsset::GetAssetId() const
	{
		return metaData->GetAssetID();
	}

	SAsset* SAsset::Import(const fs::path filePath)
	{
		if (IsAssetFile(filePath))
		{
			const SAssetMetadata tmpMetadata(filePath);
			const SAssetType assetType = tmpMetadata.GetAssetType();
			auto newAsset = Create(filePath, assetType);
			if (newAsset && newAsset->Import())
				return newAsset;
		}

		return nullptr;
	}

	bool SAsset::IsAssetFile(const fs::path filePath)
	{
		if (!FileUtils::FileExists(filePath))
			return false;

		if (!StringCompareCase(filePath.extension().string(), ASSET_EXT))
			return false;

		return true;
	}

	SAsset* SAsset::Create(const fs::path filePath, const SAssetType type)
	{
		switch (type)
		{
		case SAssetType::NONE:
			return nullptr;
		case SAssetType::TEXTURE:
			break;
		case SAssetType::MESH:
			break;
		case SAssetType::SHADER:
			return new ShaderAsset(filePath);
		case SAssetType::MATERIAL:
			break;
		case SAssetType::SOUND:
			break;
		case SAssetType::LEVEL:
			break;
		case SAssetType::OBJECT:
			break;
		default:
			return nullptr;
		}
		return nullptr;
	}

}
