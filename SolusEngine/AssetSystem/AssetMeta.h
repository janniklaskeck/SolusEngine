#pragma once

#include "Engine/SolusEngine.h"

#include <nlohmann\json.hpp>

namespace Solus
{

	class SOLUS_API AssetMeta
	{
	public:
		AssetMeta();
		void Initialize(class Asset* asset);
		void ReadMetaData(std::string& path);
		void CreateMetaData(std::string& path);

	private:
		nlohmann::json metaData;
	};

}
