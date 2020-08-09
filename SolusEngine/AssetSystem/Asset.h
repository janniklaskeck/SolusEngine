#pragma once

#include "Asset.generated.h"
#include "Engine/SolusEngine.h"
#include "Object/SolusObject.h"
#include "Utility/Vector.h"
#include "Utility/RTTI.h"

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

	SOLUS_CLASS();
	class SOLUS_API Asset : public SolusObject
	{
		META(Asset, SolusObject)
	public:

		Asset();
		virtual ~Asset();
		
		virtual void Initialize(std::filesystem::path filePath, bool forceLoad = false);

		virtual void Load();
		virtual void Unload();

		void* GetRawData() const;
		uintmax_t GetDataSize(bool readFromFile = false) const;

		std::filesystem::path GetFilePath(bool relativePath = false) const;
		std::string GetFileName(bool removeExtension = false) const;

		std::string GetFileType() const;

		const uint32_t GetAssetId() const;

		virtual void PostSerialize() override;

	protected:
		SPROPERTY();
		uint32_t assetId = 0;

		std::filesystem::path path;
		AssetType type;

		std::unique_ptr<class AssetMeta> metaData;

		void* dataPtr;
		uintmax_t dataLength;
	};

}
