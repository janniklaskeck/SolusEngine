#pragma once

#include "SolusEngine.h"
#include "Vector.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

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

	class SOLUS_API Asset
	{
	public:
		static const char* ASSET_FILE_EXTENSION;

		Asset();
		virtual ~Asset();
		
		virtual void Initialize(const char* filePath, bool forceLoad = false);

		virtual void Load();
		virtual void Unload();

		void* GetRawData() const;
		uintmax_t GetDataSize() const;

		std::string GetFilePath() const;
	protected:
		std::string path;
		AssetType type;

		std::unique_ptr<class AssetMeta> metaData;

		void* dataPtr;
		uintmax_t dataLength;
	};

}
