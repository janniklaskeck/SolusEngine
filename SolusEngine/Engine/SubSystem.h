#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class SOLUS_API SubSystem
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Destroy() = 0;
	};
}
