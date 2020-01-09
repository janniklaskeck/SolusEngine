#pragma once

#include "Engine/SolusEngine.h"

#include "SolusObject.generated.h"

#include "Utility/RTTI.h"

namespace Solus
{

	SOLUS_CLASS();
	class SOLUS_API SolusObject
	{
		REFLECT(SolusObject)
	public:
		virtual void Begin();
		virtual void End();
	};
}
