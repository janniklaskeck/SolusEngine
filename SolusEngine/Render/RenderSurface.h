#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{


	class SOLUS_API RenderSurface
	{
	public:
		virtual void Initialize(int width, int height) = 0;

		virtual unsigned int GetFramebuffer() const = 0;
		virtual unsigned int GetDepthBuffer() const = 0;
		virtual unsigned int GetColorTexture() const = 0;

		virtual void GetSize(int& width, int& height) = 0;
	};

}
