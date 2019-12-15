#pragma once
#include "AssetSource.h"

namespace Solus
{
	class Asset;

	class SOLUS_API FolderAssetSource : public AssetSource
	{
	public:
		FolderAssetSource(std::string root);

		void Initialize() override;
		
		Asset* GetAsset(std::string& path);

	};

}
