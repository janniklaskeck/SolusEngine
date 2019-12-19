#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{


	class SOLUS_API RenderSurface
	{
		virtual void Initialize(int width, int height) = 0;

		virtual void GetSize(int& width, int& height) = 0;
	};

}
