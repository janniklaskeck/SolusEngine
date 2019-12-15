#pragma once
#include "Asset.h"

namespace Solus
{

	class SOLUS_API TextAsset : public Asset
	{
	public:		
		std::string GetStringContent() const;
		const char* GetCharContent() const;
	};

}
