#include "OpenGLImage.h"
#include "FileUtils.h"

#include <GL/gl3w.h>
#include <IL/devil_cpp_wrapper.hpp>

#include <string>
#include <iostream>
#include <assert.h>

namespace Solus
{
#define CHECK_OPENGL_ERROR() { \
	unsigned int err; \
	while ((err = glGetError()) != GL_NO_ERROR) \
	{		std::cout << "OPENGL ERROR: " << err << std::endl; \
            assert(0); \
	} \
}
	bool OpenGLImage::Initialize(const char* imagePath)
	{
		
		uint32_t devilImageID;
		ilGenImages(1, &devilImageID);
		ilBindImage(devilImageID);

		ILboolean result = ilLoadImage(imagePath);
		CHECK_OPENGL_ERROR();
		if (result)
		{
			int format = ilGetInteger(IL_IMAGE_FORMAT);
			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);
			bpp = ilGetInteger(IL_IMAGE_BPP);
			CHECK_OPENGL_ERROR();
			imageID = ilutGLBindMipmaps();
			//imageID = ilutGLBindTexImage();

			//glBindTexture(GL_TEXTURE_2D, imageID);
			CHECK_OPENGL_ERROR();
			int mipMapLevel = 0;
			while (ilActiveMipmap(mipMapLevel))
			{
				int curWidth = ilGetInteger(IL_IMAGE_WIDTH);
				int curHeight = ilGetInteger(IL_IMAGE_HEIGHT);
				ILint size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);;
				void* data = ilGetData();
				glCompressedTexImage2D(GL_TEXTURE_2D, mipMapLevel, format, curWidth, curHeight, 0, size, data);
				CHECK_OPENGL_ERROR();
				++mipMapLevel;
			}
			
			ilDeleteImages(1, &devilImageID);
		}

		return result;
	}

}
