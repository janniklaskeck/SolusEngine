#include "Asset.h"
#include "AssetMeta.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

namespace Solus
{
	SCLASS_IMPL(Asset);

	Asset::Asset()
		: dataPtr(nullptr), dataLength(0L), path(""), type(AssetType::AT_UNKNOWN)
	{}

	Asset::~Asset()
	{}

	void Asset::Initialize(std::filesystem::path filePath, bool forceLoad/* = false*/)
	{
		this->path = filePath;
		metaData.reset(new AssetMeta);
		metaData->Initialize(this);
		assetId = metaData->GetMetaId();
	}

	void Asset::Load()
	{
		if (dataPtr)
		{
			return;
		}
		dataPtr = FileUtils::ReadFileRaw(GetFilePath().c_str(), dataLength);
	}

	void Asset::Unload()
	{
		if (dataPtr)
			delete dataPtr;
		dataPtr = nullptr;
	}

	std::filesystem::path Asset::GetFilePath(bool relativePath /*= false*/) const
	{
		//if (relativePath)
		//{
		//	const auto& root = gEngine->GetAssetManager()->GetEngineAssetRoot();
		//	auto relPath = std::filesystem::relative(path, root);
		//	return relPath;
		//}
		return path;
	}

	void* Asset::GetRawData() const
	{
		return dataPtr;
	}

	uintmax_t Asset::GetDataSize(bool readFromFile /*= false*/) const
	{
		if (readFromFile)
			return std::filesystem::file_size(path);
		else
			return dataLength;
	}

	std::string Asset::GetFileName(bool removeExtension /*= false*/) const
	{
		if (removeExtension)
		{
			return path.stem().string();
		}
		return path.filename().string();
	}

	std::string Asset::GetFileType() const
	{
		std::string fileType = path.filename().extension().string();
		ToLower(fileType);
		return fileType;
	}

	const uint32_t Asset::GetAssetId() const
	{
		return assetId;
	}

	void Asset::PostSerialize()
	{

	}

}
