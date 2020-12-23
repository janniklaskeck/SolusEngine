#pragma once

#include "ShaderAsset.generated.h"
#include "Engine/SolusEngine.h"
#include "AssetSystem/SAsset.h"

namespace Solus
{
	class RenderShader;

	SOLUS_CLASS();
	class SOLUS_API ShaderAsset : public SAsset
	{
		META(ShaderAsset, SAsset)
	public:

		ShaderAsset(const fs::path shaderPath);
		virtual ~ShaderAsset() = default;

		void Load() override;
		void Unload() override;

		bool Import() override;

		static SAssetType GetAssetType()
		{
			return SAssetType::SHADER;
		}

		const unsigned char* GetShaderContent() const;

		uint32_t GetShaderSize() const;

	protected:

		std::vector<unsigned char> rawBytes;

		RenderShader* renderShader = nullptr;
	};

}
