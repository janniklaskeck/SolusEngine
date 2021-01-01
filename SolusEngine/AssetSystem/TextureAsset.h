#pragma once

#include "TextureAsset.generated.h"
#include "Engine/SolusEngine.h"
#include "AssetSystem/SAsset.h"

namespace Solus
{

	class RenderTexture;

	SOLUS_CLASS();
	class SOLUS_API TextureAsset : public SAsset
	{
		META(TextureAsset, SAsset)
	public:

		TextureAsset(const fs::path shaderPath);
		virtual ~TextureAsset() = default;

		void Load() override;
		void Unload() override;

		bool Import() override;

		static const std::pair<std::string, std::string> GetAssetFileTypeFilter();

		RenderTexture* GetRenderTexture() const;

		const std::vector<unsigned char>& GetTextureData() const;

	protected:

		RenderTexture* renderTexture = nullptr;
		std::vector<unsigned char> data;

	};

}
