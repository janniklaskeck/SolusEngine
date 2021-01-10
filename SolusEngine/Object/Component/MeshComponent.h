#pragma once

#include "MeshComponent.generated.h"

#include "SComponent.h"

#include "AssetSystem/MeshAsset.h"
#include "AssetSystem/TextureAsset.h"

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

	public:
		SPROPERTY();
		MeshAsset* meshAsset = nullptr;

		// Used to detect mesh asset changes
		MeshAsset* oldMeshAsset = nullptr;

		SPROPERTY();
		std::vector<TextureAsset*> textureAssets;

		RenderMesh* renderMesh = nullptr;
	};
}