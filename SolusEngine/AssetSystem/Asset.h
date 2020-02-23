#pragma once

#include "Engine/SolusEngine.h"
#include "Utility/Vector.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <filesystem>

namespace Solus
{
	enum class SOLUS_API AssetType : uint8_t
	{
		AT_TEXT = 0,
		AT_TEXTURE,
		AT_MESH,
		AT_SOUND,
		AT_UNKNOWN
	};

#define ASSET_FILE_EXTENSION ".asset"

	class SOLUS_API Asset
	{
	public:

		Asset();
		virtual ~Asset();
		
		virtual void Initialize(std::filesystem::path filePath, bool forceLoad = false);

		virtual void Load();
		virtual void Unload();

		void* GetRawData() const;
		uintmax_t GetDataSize() const;

		std::filesystem::path GetFilePath() const;
		std::string GetFileName() const;
	protected:
		std::filesystem::path path;
		AssetType type;

		std::unique_ptr<class AssetMeta> metaData;

		void* dataPtr;
		uintmax_t dataLength;
	};

}
