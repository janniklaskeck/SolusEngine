#pragma once

namespace Solus
{

	class SShaderProgramAsset final : public SAsset
	{
	public:

		SShaderProgramAsset() = default;

		SShaderProgramAsset(const SAssetPath& InPath)
			: SAsset(InPath)
		{
		}

		void Load() override;

		void Unload() override;

		bool IsValid() const override;

		bool IsLoaded() const override;

	public:

		void SetPaths(const SAssetPath& InVertexShaderPath, const SAssetPath& InFragmentShaderPath);

		SAssetPath GetVertexShaderPath() const
		{
			return VertexShaderPath;
		}

		SAssetPath GetFragmentShaderPath() const
		{
			return FragmentShaderPath;
		}

	private:

		SAssetPath VertexShaderPath;
		SAssetPath FragmentShaderPath;
	};

}