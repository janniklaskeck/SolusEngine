#pragma once

#include "Engine/SolusEngine.h"
#include "Asset.h"

#include "Utility/Helper.h"

#include <string>
#include <unordered_map>
#include <algorithm>
#include <filesystem>

namespace Solus
{

	class Asset;

	class SOLUS_API AssetSource
	{
	public:

		AssetSource(std::string root);

		virtual void Initialize() = 0;

		virtual Asset* GetAsset(std::string& path) = 0;

		std::unordered_map<std::string, Asset*>::const_iterator BeginIter() const;

		std::unordered_map<std::string, Asset*>::const_iterator EndIter() const;

	protected:
		std::filesystem::path root;

		void InitializeAsset(std::filesystem::path relativePath);

		void CleanPath(std::string& path);
	private:
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

		std::unordered_map<std::string, AssetType> extensionTypeMap;
	protected:
		std::unordered_map<std::string, Asset*, case_insensitive_hasher, case_insensitive_comparer> assets;
	};

}
