#include "ShaderAsset.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

namespace Solus
{

	ShaderAsset::ShaderAsset(const fs::path shaderPath)
		: SAsset(shaderPath)
	{
		
	}

	void ShaderAsset::Load()
	{
	}

	void ShaderAsset::Unload()
	{

	}

	bool ShaderAsset::Import()
	{

		return true;
	}

}
