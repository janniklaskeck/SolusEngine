#pragma once

#include "MeshComponent.generated.h"

#include "SComponent.h"
#include "AssetSystem/MeshAsset.h"

namespace Solus
{
	class RenderMesh;

	SOLUS_CLASS();
	class SOLUS_API MeshComponent : public SComponent
	{
		META(MeshComponent, SComponent)
	public:
		virtual void BeginPlay() override;
		virtual void Update(float deltaTime) override;
		virtual void Render();
		virtual void EndPlay() override;

		void SetMesh(MeshAsset* meshAsset);
	public:
		SPROPERTY();
		MeshAsset* meshAsset = nullptr;
	};
}