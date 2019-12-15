#include "Asset.h"
#include "AssetMeta.h"
#include "Engine.h"
#include "FileUtils.h"

namespace Solus
{
	const char* Asset::ASSET_FILE_EXTENSION = ".asset";

	Asset::Asset()
		: dataPtr(nullptr), dataLength(0L), path(""), type(AssetType::AT_UNKNOWN)
	{}

	Asset::~Asset()
	{}

	void Asset::Initialize(const char* filePath, bool forceLoad/* = false*/)
	{
		this->path = filePath;
		metaData.reset(new AssetMeta);
		metaData->Initialize(this);
	}

	void Asset::Load()
	{
		if (dataPtr)
		{
			gEngine->Log(LogWarning, "Trying to load loaded Asset: %s", GetFilePath().c_str());
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

	std::string Asset::GetFilePath() const
	{
		return std::string(path);
	}

	void* Asset::GetRawData() const
	{
		return dataPtr;
	}

	uintmax_t Asset::GetDataSize() const
	{
		return dataLength;
	}

}
