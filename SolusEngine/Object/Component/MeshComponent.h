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
		virtual void BeginPlay() override;
		virtual void Update(float deltaTime) override;
		virtual void Render();
		virtual void EndPlay() override;

		void SetMesh(Asset meshAsset);
	public:
		SPROPERTY();
		Asset meshAsset;

		std::unique_ptr<RenderMesh> renderMesh = nullptr;
	};
}