#include "ShaderAsset.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

namespace Solus
{

	ShaderAsset::ShaderAsset(const fs::path shaderPath)
		: SAsset(shaderPath, SAssetType::SHADER)
	{

	}

	void ShaderAsset::Load()
	{
		if (rawBytes.empty())
		{
			fs::path sourcePath = metaData->GetSourceFilePath();
			FileUtils::ReadFileRaw(sourcePath, rawBytes);
			if (rawBytes[rawBytes.size() - 1] != '\0')
				rawBytes.push_back('\0');

			renderShader = gEngine->GetRenderDevice()->CreateShader(*this);
		}
	}

	void ShaderAsset::Unload()
	{
		if (!rawBytes.empty())
			rawBytes.clear();
	}

	bool ShaderAsset::Import()
	{

		return true;
	}

	const std::pair<std::string, std::string> ShaderAsset::GetAssetFileTypeFilter()
	{
		return { "OpenGL Shader (.glsl)", "*.glsl" };
	}

	const unsigned char* ShaderAsset::GetShaderContent() const
	{
		return rawBytes.data();
	}

	uint32_t ShaderAsset::GetShaderSize() const
	{
		return rawBytes.size();
	}

	RenderShader* ShaderAsset::GetRenderShader() const
	{
		return renderShader;
	}

}
