#include "render/SShaderProgramCache.hpp"

#include "assets/SAssetManager.hpp"
#include "tools/SShaderCompiler.hpp"

namespace Solus
{

	bgfx::ProgramHandle SShaderProgramCache::GetProgramHandle(const Ref<SShaderProgramAsset>& ProgramAsset)
	{
		if (!ProgramAsset->IsValid())
		{
			return BGFX_INVALID_HANDLE;
		}

		const SAssetPath& VertexShaderPath = ProgramAsset->GetVertexShaderPath();
		const SAssetPath& FragmentShaderPath = ProgramAsset->GetFragmentShaderPath();

		ShaderPathPair Key{ VertexShaderPath, FragmentShaderPath };

		auto Iter = ProgramCache.find(Key);
		if (Iter != ProgramCache.end())
		{
			bgfx::ProgramHandle Handle = (*Iter).second;
			return Handle;
		}

		bgfx::ShaderHandle VertexShaderHandle = GetShaderHandle(VertexShaderPath, false);
		bgfx::ShaderHandle FragmentShaderHandle = GetShaderHandle(FragmentShaderPath, true);

		bgfx::ProgramHandle NewHandle = bgfx::createProgram(VertexShaderHandle, FragmentShaderHandle, false);

		ProgramCache[Key] = NewHandle;

		return NewHandle;
	}

	bgfx::ShaderHandle SShaderProgramCache::GetShaderHandle(SAssetPath ShaderFilepath, const bool bIsFragment)
	{
		auto Iter = ShaderCache.find(ShaderFilepath);
		if (Iter != ShaderCache.end())
		{
			return (*Iter).second;
		}

		SAssetPath ShaderBinaryPath;
		if (!SShaderCompiler::CompileShader(ShaderFilepath, bIsFragment ? SShaderType::Fragment : SShaderType::Vertex, ShaderBinaryPath))
		{
			return BGFX_INVALID_HANDLE;
		}

		std::vector<uint8> Buffer;
		SAssetManager::Get().ReadFile(ShaderBinaryPath, Buffer);

		bgfx::ShaderHandle NewShaderHandle = bgfx::createShader(bgfx::copy(Buffer.data(), Buffer.size()));

		ShaderCache[ShaderFilepath] = NewShaderHandle;

		return NewShaderHandle;
	}

}
