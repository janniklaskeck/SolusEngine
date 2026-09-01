#pragma once

#include "bgfx/bgfx.h"
#include "SEntityComponent.hpp"

namespace Solus
{
	class SOLUS_API SMeshComponent : public SEntityComponent
	{
	public:

		std::vector<std::pair<SString, void*>> GetUniformData() const;

		Ref<SMeshAsset> MeshAsset;

		Ref<SShaderProgramAsset> ShaderProgramAsset;

		SFColor Color{};
	};
}	