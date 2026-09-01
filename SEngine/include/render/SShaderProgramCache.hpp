#pragma once

namespace Solus
{
	class SShaderProgramCache
	{
	public:

		struct ShaderPathPair
		{
			SAssetPath VertexShaderPath;
			SAssetPath FragmentShaderPath;

			bool operator==(const ShaderPathPair& Other) const
			{
				return (VertexShaderPath == Other.VertexShaderPath
					&& FragmentShaderPath == Other.FragmentShaderPath);
			}
		};

		struct KeyHasher
		{
			std::size_t operator()(const ShaderPathPair& k) const
			{
				return ((std::hash<SAssetPath>()(k.VertexShaderPath)
					^ (std::hash<SAssetPath>()(k.FragmentShaderPath) << 1)) >> 1);
			}
		};

	public:

		bgfx::ProgramHandle GetProgramHandle(const Ref<SShaderProgramAsset>& ProgramAsset);

	private:

		bgfx::ShaderHandle GetShaderHandle(SAssetPath ShaderFilepath, const bool bIsFragment);

	private:

		std::unordered_map<ShaderPathPair, bgfx::ProgramHandle, KeyHasher> ProgramCache;
		std::unordered_map<SAssetPath, bgfx::ShaderHandle> ShaderCache;
	};

}
