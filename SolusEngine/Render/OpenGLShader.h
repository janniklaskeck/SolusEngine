#pragma once
#include "RenderShader.h"

namespace Solus
{
	class OpenGLShader : public RenderShader
	{
	public:
		OpenGLShader();
		~OpenGLShader();

		virtual bool Load(const Asset& shaderAsset) override;


		void Bind();
		unsigned int GetUniformLocation(const char* name);

		virtual unsigned int GetShaderProgram() const override;
	private:

		unsigned int shaderProgram = 0;
	};

}
