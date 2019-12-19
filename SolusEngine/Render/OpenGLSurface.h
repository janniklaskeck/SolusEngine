#pragma once

#include "RenderSurface.h"

namespace Solus
{

	class OpenGLSurface : public RenderSurface
	{
	public:

		~OpenGLSurface();
		virtual void Initialize(int width, int height) override;

		virtual void GetSize(int& width, int& height) override;

		void BindBuffers();

		unsigned int GetFramebuffer() const;
		unsigned int GetDepthBuffer() const;
		unsigned int GetColorTexture() const;
	private:
		int width;
		int height;

		unsigned int framebufferId;
		unsigned int colorBufferId;
		unsigned int depthBufferId;

		unsigned int drawBuffers[1];
	};

}
