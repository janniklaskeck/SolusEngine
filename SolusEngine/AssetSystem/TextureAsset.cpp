#include "TextureAsset.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

#include "Render/RenderTexture.h"

namespace Solus
{

	TextureAsset::TextureAsset(const fs::path filePath)
		: SAsset(filePath, SAssetType::TEXTURE)
	{

	}

	void TextureAsset::Load()
	{
		FileUtils::ReadFileRaw(GetSourceFilePath(), data);
		if (!renderTexture)
			renderTexture = gEngine->GetRenderDevice()->CreateTexture(*this);
	}

	void TextureAsset::Unload()
	{
		if (renderTexture)
		{
			data.clear();
			gEngine->GetRenderDevice()->DestroyTexture(renderTexture);
			renderTexture = nullptr;
		}
	}

	bool TextureAsset::Import()
	{

		return true;
	}

	const std::pair<std::string, std::string> TextureAsset::GetAssetFileTypeFilter()
	{
		return { "Texture (.dds)", "*.dds" };
	}

	RenderTexture* TextureAsset::GetRenderTexture() const
	{
		return renderTexture;
	}

	const std::vector<unsigned char>& TextureAsset::GetTextureData() const
	{
		return data;
	}

}
