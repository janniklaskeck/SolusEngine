#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class ShaderAsset;

	class SOLUS_API RenderShader
	{
	public:
		RenderShader();
		virtual ~RenderShader();

		virtual bool Load(ShaderAsset& shaderAsset) = 0;

		virtual unsigned int GetShaderProgram() const = 0;
	};

}
