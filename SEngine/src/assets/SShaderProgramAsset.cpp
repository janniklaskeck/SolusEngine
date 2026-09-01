#include "assets/SShaderProgramAsset.hpp"

#include "../../include/assets/SAssetManager.hpp"

namespace Solus
{
	void SShaderProgramAsset::Load()
	{
		// Don't load anything, loading is handled by the renderer
	}

	void SShaderProgramAsset::Unload()
	{
		// Don't load anything, unloading is handled by the renderer
	}

	bool SShaderProgramAsset::IsValid() const
	{
		const bool bIsVertexPathValid = SAssetManager::Get().IsValidPath(VertexShaderPath);
		const bool bIsFragmentPathValid = SAssetManager::Get().IsValidPath(FragmentShaderPath);

		return bIsVertexPathValid && bIsFragmentPathValid;
	}

	bool SShaderProgramAsset::IsLoaded() const
	{
		return true;
	}

	void SShaderProgramAsset::SetPaths(const SAssetPath& InVertexShaderPath, const SAssetPath& InFragmentShaderPath)
	{
		VertexShaderPath = InVertexShaderPath;
		FragmentShaderPath = InFragmentShaderPath;
	}
}
