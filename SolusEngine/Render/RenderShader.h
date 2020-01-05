#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class TextAsset;

	class SOLUS_API RenderShader
	{
	public:
		RenderShader();
		virtual ~RenderShader();

		virtual bool Load(TextAsset* vertexShaderFilePath, TextAsset* fragmentShaderFilePath) = 0;

		virtual unsigned int GetShaderProgram() const = 0;
	};

}
