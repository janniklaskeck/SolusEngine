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

	public:
		SPROPERTY();
		Asset meshAsset;

		// Used to detect mesh asset changes
		Asset oldMeshAsset;

		SPROPERTY();
		std::vector<Asset> textureAssets;

		RenderMesh* renderMesh = nullptr;
	};
}