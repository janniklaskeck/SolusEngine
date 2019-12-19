#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class SOLUS_API Window
	{
	public:

		virtual void Initialize();
		virtual void Run();
		virtual void Update();
		virtual void Close();
	};
}
