#pragma once
#include "RenderTexture.h"
#include "Engine/SolusEngine.h"

#include <cstdint>

namespace Solus
{
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
	
	class OpenGLTexture : public RenderTexture
	{
	public:
		OpenGLTexture(unsigned int textureType);

		~OpenGLTexture();

		void Bind(unsigned int textureIndex) override;

		bool Load(TextureAsset& textureAsset) override;

		uint32_t GetImageID()
		{
			return textureID;
		}

	private:
		Asset asset;
		uint32_t textureID = 0;
		unsigned int textureType = 0;

		unsigned int GetGLTextureUnit(int index);
	};

}
