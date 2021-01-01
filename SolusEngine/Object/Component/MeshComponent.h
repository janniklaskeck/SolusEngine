#pragma once

#include "MeshComponent.generated.h"

#include "SComponent.h"
#include "AssetSystem/Asset.h"

namespace Solus
{
	class RenderMesh;

	SOLUS_CLASS();
	class SOLUS_API MeshComponent : public SComponent
	{
		META(MeshComponent, SComponent)
	public:
		void BeginPlay() override;
		void Update(float deltaTime) override;
		void Render();
		void EndPlay() override;

		void SetMesh(Asset meshAsset);

		void SetTexture(uint8_t index, Asset textureAsset);
	public:
		SPROPERTY();
		Asset meshAsset;

		SPROPERTY();
		std::vector<Asset> textureAssets;

		std::unique_ptr<RenderMesh> renderMesh = nullptr;
	};
}