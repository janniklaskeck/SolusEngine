#include "OpenGLSurface.h"
#include "OpenGLDevice.h"

#include "Engine/Engine.h"

#include <GL/gl3w.h>

namespace Solus
{
	OpenGLSurface::~OpenGLSurface()
	{
		glDeleteFramebuffers(1, &framebufferId);
		glDeleteRenderbuffers(1, &depthBufferId);
		glDeleteTextures(1, &colorBufferId);
	}

	void OpenGLSurface::Initialize(int width, int height)
	{
		this->width = width;
		this->height = height;
		glGenFramebuffers(1, &framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &colorBufferId);
		glBindTexture(GL_TEXTURE_2D, colorBufferId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferId, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &depthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);



		drawBuffers[0] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			gEngine->Log(LogLevel::LogError, "Framebuffer is not complete!");
			assert(false);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CHECK_OPENGL_ERROR();
	}

	void OpenGLSurface::GetSize(int& width, int& height)
	{
		width = this->width;
		height = this->height;
	}

	void OpenGLSurface::BindBuffers()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.f, 1.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CHECK_OPENGL_ERROR();
	}

	unsigned int OpenGLSurface::GetFramebuffer() const
	{
		return framebufferId;
	}
	unsigned int OpenGLSurface::GetDepthBuffer() const
	{
		return depthBufferId;
	}

	unsigned int OpenGLSurface::GetColorTexture() const
	{
		return colorBufferId;
	}
}
