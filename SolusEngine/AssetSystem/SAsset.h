#pragma once

#include "SAsset.generated.h"
#include "SAssetType.h"
#include "SAssetMetadata.h"

#include "Engine/SolusEngine.h"

#include "Object/SObject.h"

#include "Utility/Vector.h"
#include "Utility/RTTI.h"
#include "Utility/Helper.h"
#include "Utility/FileUtils.h"

#include "ReferenceCounted.h"

#include "Utility/BinaryReader.h"

#include <nlohmann/json.hpp>


namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API SAsset : public SObject, public ReferenceCounted
	{
		META(SAsset, SObject)
	public:

		SAsset(const fs::path filePath);
		SAsset(const fs::path filePath, const SAssetType assetType);
		virtual ~SAsset() = default;

		virtual void Load() = 0;
		virtual void Unload() = 0;

		virtual bool Import() = 0;

		const fs::path GetAssetFilePath() const;

		const fs::path GetSourceFilePath() const;

		const std::string GetFileName() const;

		const SAssetType GetAssetType() const;

		const SUUID GetAssetId() const;

		template<typename T>
		static std::enable_if_t<std::is_base_of_v<SAsset, T>, T*> Import(const fs::path filePath);

		static SAsset* Import(const fs::path filePath);

		static bool IsAssetFile(const fs::path filePath);
	private:
		static SAsset* Create(const fs::path filePath, const SAssetType type);

	public:
		static const std::string ASSET_EXT;

	private:
		bool LoadMetadata();

	protected:
		std::unique_ptr<SAssetMetadata> metaData;
		fs::path assetFilePath;
	};

	template<typename T>
	static std::enable_if_t<std::is_base_of_v<SAsset, T>, T*>
		Solus::SAsset::Import(const fs::path filePath)
	{
		auto* newInstance = new T(filePath);
		if (newInstance->Import())
			return newInstance;
		delete newInstance;
		return nullptr;
	}

}
