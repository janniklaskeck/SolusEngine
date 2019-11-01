#pragma once
#include "RenderImage.h"
#include "SolusEngine.h"

#include <cstdint>

namespace Solus
{

	class OpenGLImage : public RenderImage
	{
	public:
		OpenGLImage() {}

		virtual bool Initialize(const char* imagePath) override;

		uint32_t imageID = 0;
	};

}
