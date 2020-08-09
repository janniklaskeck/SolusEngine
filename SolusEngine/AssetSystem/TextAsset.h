#pragma once
#include "TextAsset.generated.h"
#include "Asset.h"

namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API TextAsset : public Asset
	{
		META(TextAsset, Asset);
	public:		
		std::string GetStringContent() const;
		const char* GetCharContent() const;
	};

}
