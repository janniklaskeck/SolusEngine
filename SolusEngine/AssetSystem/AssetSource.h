#pragma once

#include "Engine/SolusEngine.h"
#include "SAsset.h"

#include "Utility/Helper.h"
#include "Utility/FileUtils.h"

#include <unordered_map>
#include <algorithm>

namespace Solus
{
	class EditorAssetWindow;

	class SOLUS_API AssetSource
	{
		friend EditorAssetWindow;

	public:

		AssetSource(std::string root);

		virtual void Initialize() = 0;

		virtual void Refresh() = 0;

		SAsset* GetAssetFromPath(const std::string& path);
		SAsset* GetAsset(const SUUID id);

		const fs::path& GetRootPath() const;

		void InitializeAsset(const fs::path relativePath);

		void InitializeAsset(SAsset* importedAsset, const fs::path relativePath);

	protected:
		fs::path root;

		void CleanPath(std::string& path);

	private:

		struct IDHash
		{
			size_t operator()(const SUUID& key) const
			{
				return std::hash<uint32_t>()(key.GetRaw());
			}
		};

		struct case_insensitive_hasher
		{
			size_t operator()(const std::string& key) const
			{
				std::string keyCopy(key);
				ToLower(keyCopy);
				return std::hash<std::string>()(keyCopy);
			}
		};

		struct case_insensitive_comparer
		{
			bool operator () (const std::string& x, const std::string& y) const
			{
				return _stricmp(x.c_str(), y.c_str()) == 0;
			}
		};

	protected:
		std::unordered_map<std::string, SAsset*, case_insensitive_hasher, case_insensitive_comparer> pathAssets;
		std::unordered_map<SUUID, SAsset*, IDHash> idAssets;
	};

}
