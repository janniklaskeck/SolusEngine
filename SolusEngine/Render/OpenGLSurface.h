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

		virtual unsigned int GetFramebuffer() const override;
		virtual unsigned int GetDepthBuffer() const override;
		virtual unsigned int GetColorTexture() const override;
	private:
		int width;
		int height;

		unsigned int framebufferId;
		unsigned int colorBufferId;
		unsigned int depthBufferId;

		unsigned int drawBuffers[1];
	};

}
