#pragma once

#include "Engine/SolusEngine.h"

#include <nlohmann\json.hpp>

namespace Solus
{

	class SOLUS_API AssetMeta
	{
	public:
		AssetMeta();
		void Initialize(const class Asset* asset);
		void ReadMetaData(const std::string& path);
		void WriteMetaData(const std::string& path);

		uint32_t GetMetaId() const;

	private:
		nlohmann::json metaData;
	};

}
