#include "OpenGLTexture.h"
#include "Utility/FileUtils.h"
#include "Utility/BinaryReader.h"

#include "AssetSystem/TextureAsset.h"

#include <GL/gl3w.h>
#include <cassert>

namespace Solus
{
	OpenGLTexture::OpenGLTexture(unsigned int textureType)
		: textureType(textureType)
	{}

	OpenGLTexture::~OpenGLTexture()
	{
		if (textureID > 0)
		{
			glDeleteTextures(1, &textureID);
		}
	}

	void OpenGLTexture::Bind(unsigned int textureIndex)
	{
		glActiveTexture(GetGLTextureUnit(textureIndex));
		glBindTexture(textureType, textureID);
	}

	bool OpenGLTexture::Load(TextureAsset& textureAsset)
	{
		BinaryReader reader(textureAsset.GetTextureData().data(), textureAsset.GetTextureData().size());
		auto filecode = reader.ReadString(4);

		FILE* fp = nullptr;
		if (strncmp(filecode.c_str(), "DDS ", 4) != 0)
		{
			// Not a dds file
			return false;
		}

		/* get the surface desc */
		unsigned char header[124];
		reader.ReadBytes(header, 124);

		unsigned int height = *(unsigned int*)&(header[8]);
		unsigned int width = *(unsigned int*)&(header[12]);
		unsigned int linearSize = *(unsigned int*)&(header[16]);
		unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		unsigned int fourCC = *(unsigned int*)&(header[80]);


		unsigned char* buffer;
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		reader.ReadBytes(buffer, bufsize);

		unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return false;
		}

		// Create one OpenGL texture
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if (width < 1) 
				width = 1;
			if (height < 1) 
				height = 1;

		}
		free(buffer);
		return true;
	}

	unsigned int OpenGLTexture::GetGLTextureUnit(int index)
	{
		switch (index)
		{
		case 0:
			return GL_TEXTURE0;
		case 1:
			return GL_TEXTURE1;
		case 2:
			return GL_TEXTURE2;
		case 3:
			return GL_TEXTURE3;
		case 4:
			return GL_TEXTURE4;
		case 5:
			return GL_TEXTURE5;
		case 6:
			return GL_TEXTURE6;
		case 7:
			return GL_TEXTURE7;
		default:
			return GL_TEXTURE0;
		}
	}
}
