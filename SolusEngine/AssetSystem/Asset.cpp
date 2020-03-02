#include "Asset.h"
#include "AssetMeta.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

namespace Solus
{
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
	}

	void Asset::Load()
	{
		if (dataPtr)
		{
			gEngine->Log(LogLevel::LogWarning, "Trying to load loaded Asset: %s", GetFilePath().string().c_str());
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

	std::filesystem::path Asset::GetFilePath() const
	{
		return path;
	}

	void* Asset::GetRawData() const
	{
		return dataPtr;
	}

	uintmax_t Asset::GetDataSize() const
	{
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

}
