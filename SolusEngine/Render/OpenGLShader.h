#pragma once
#include "RenderShader.h"

namespace Solus
{
	class OpenGLShader : public RenderShader
	{
	public:
		OpenGLShader();
		~OpenGLShader();

		virtual bool Load(TextAsset* vertexShaderFilePath, TextAsset* fragmentShaderFilePath) override;


		void Bind();
		unsigned int GetUniformLocation(const char* name);

		virtual unsigned int GetShaderProgram() const override;
	private:

		unsigned int shaderProgram = 0;
	};

}
