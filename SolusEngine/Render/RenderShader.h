#pragma once

#include "Engine/SolusEngine.h"
#include "AssetSystem/Asset.h"

namespace Solus
{
	class SOLUS_API RenderShader
	{
	public:
		RenderShader();
		virtual ~RenderShader();

		virtual bool Load(const Asset& shaderAsset) = 0;

		virtual unsigned int GetShaderProgram() const = 0;
	};

}
