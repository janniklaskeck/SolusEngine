#pragma once
#include "RenderShader.h"

namespace Solus
{
	class OpenGLShader : public RenderShader
	{
	public:
		OpenGLShader();
		~OpenGLShader();

		bool Load(ShaderAsset& shaderAsset) override;


		void Bind();
		unsigned int GetUniformLocation(const char* name);

		unsigned int GetShaderProgram() const override;
	private:

		unsigned int shaderProgram = 0;
	};

}
