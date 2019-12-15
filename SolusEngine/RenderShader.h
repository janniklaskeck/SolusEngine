#pragma once

namespace Solus
{
	class TextAsset;

	class RenderShader
	{
	public:
		RenderShader();
		virtual ~RenderShader();

		virtual bool Load(TextAsset* vertexShaderFilePath, TextAsset* fragmentShaderFilePath) = 0;

		virtual unsigned int GetShaderProgram() const = 0;
	};

}
