#pragma once
#include "RenderMesh.h"
#include "OpenGLImage.h"

namespace Solus
{
	class OpenGLMesh : public RenderMesh
	{
	public:
		OpenGLMesh();
		~OpenGLMesh();
		virtual void Render() override;

	private:

		class OpenGLShader* GetOpenGLShader() const;
		unsigned int vertexbuffer, VAO, IBO;
		unsigned int uvbuffer;
		unsigned int mvpUniformLoc;
		unsigned int textureSamplerLoc;

		OpenGLImage* image;

	};
}
