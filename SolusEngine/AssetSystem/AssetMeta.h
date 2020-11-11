#pragma once

#include "Engine/SolusEngine.h"

#include <nlohmann\json.hpp>

namespace Solus
{

	class SOLUS_API AssetMeta
	{
	public:
		AssetMeta();
		void Initialize(const class SAsset* asset);
		void ReadMetaData(const std::string& path);
		void WriteMetaData(const std::string& path);

		SUUID GetMetaId() const;
	};

}
