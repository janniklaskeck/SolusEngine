#include "OpenGLPrimitiveDrawer.h"

#include "Engine/Engine.h"
#include "OpenGLDevice.h"
#include "OpenGLShader.h"

#include "Utility/Helper.h"

#include <GL/gl3w.h>

namespace Solus
{
	OpenGLPrimitiveDrawer::OpenGLPrimitiveDrawer()
	{
		Asset rectangleVS = gEngine->GetAssetManager()->GetAssetFromPath("Editor/Shader/DefaultScreen.glsl");

		rectangleShader = (OpenGLShader*)gEngine->GetRenderDevice()->CreateShader(rectangleVS);
		glGenVertexArrays(1, &rectangleVAO);
	}

	void OpenGLPrimitiveDrawer::RenderRectangle(Vec2i center, Vec2i extents, Vec4f color)
	{
		if (rectangleVBO == 0)
		{
			glGenBuffers(1, &rectangleVBO);
			glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);

			float vertices[] =
			{
				-1.f, -1.f,
				1.f, -1.f,
				-1.f, 1.f,

				-1.f, 1.f,
				1.f, -1.f,
				1.f, 1.f
			};

			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		}
		auto* renderDevice = (OpenGLDevice*)gEngine->GetRenderDevice();

		rectangleShader->Bind();
		glBindVertexArray(rectangleVAO);

		Rect rect(center, extents);
		glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
		float vertices[] = {
			(((float)rect.TopLeft().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.TopLeft().y / 720.f) - 0.5f) * 2.0f,

			(((float)rect.BottomLeft().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.BottomLeft().y / 720.f) - 0.5f) * 2.0f,

			(((float)rect.BottomRight().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.BottomRight().y / 720.f) - 0.5f) * 2.0f,

			(((float)rect.TopLeft().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.TopLeft().y / 720.f) - 0.5f) * 2.0f,

			(((float)rect.BottomRight().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.BottomRight().y / 720.f) - 0.5f) * 2.0f,

			(((float)rect.TopRight().x / 1280.f) - 0.5f) * 2.0f,
			(((float)rect.TopRight().y / 720.f) - 0.5f) * 2.0f,
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), vertices);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform4f(rectangleShader->GetUniformLocation("color"), color.r, color.g, color.b, color.a);

		// two triangles
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	void OpenGLPrimitiveDrawer::RenderCircle(Vec2i center, float radius, Vec4f color, int sections)
	{}

	void OpenGLPrimitiveDrawer::RenderSphere(Vec3f center, float radius, Vec4f color, int sections)
	{}

	void OpenGLPrimitiveDrawer::RenderBox(Vec3f center, Vec3f extents, Vec4f color)
	{}
}
