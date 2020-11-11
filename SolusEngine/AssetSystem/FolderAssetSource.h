#pragma once
#include "AssetSource.h"

#include <memory>

namespace Solus
{

	class SOLUS_API FolderAssetSource : public AssetSource
	{
	public:
		FolderAssetSource(const std::string root);

		void Initialize() override;

		void Refresh() override;
	private:
		void FindAssetInFolder(const fs::path folder);
	};

}
