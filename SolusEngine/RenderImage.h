#pragma once

#include "SolusEngine.h"

#include <cstdint>

namespace Solus
{

	class SOLUS_API RenderImage
	{
	public:
		RenderImage()
		{}
	protected:
		virtual ~RenderImage()
		{}
	public:
		virtual bool Initialize(const char* imagePath) = 0;

	protected:
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t bpp;

	};

}