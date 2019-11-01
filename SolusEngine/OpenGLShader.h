#pragma once
#include "RenderShader.h"

namespace Solus
{
	class OpenGLShader : public RenderShader
	{
	public:
		OpenGLShader();
		~OpenGLShader();

		virtual bool Load(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) override;

		virtual unsigned int GetShaderProgram() const override;

	private:

		unsigned int shaderProgram;
	};

}
