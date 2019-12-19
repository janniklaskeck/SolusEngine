#pragma once

#include "Engine/SolusEngine.h"

#include <cstdint>

namespace Solus
{
	class TextureAsset;

	class SOLUS_API RenderTexture
	{
	public:
		RenderTexture()
			:width(0), height(0), depth(0), bpp(0)
		{}
		virtual ~RenderTexture()
		{}

		virtual void Bind(unsigned int textureIndex) = 0;

		virtual bool Load(TextureAsset* textureAsset) = 0;

	protected:
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t bpp;
		bool isLoaded = false;
	};

}