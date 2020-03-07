#pragma once

#include "MeshComponent.generated.h"

#include "SComponent.h"

namespace Solus
{
	class RenderMesh;
	class MeshAsset;

	SOLUS_CLASS();
	class SOLUS_API MeshComponent : public SComponent
	{
		REFLECT(MeshComponent)
	public:
		virtual void BeginPlay() override;
		virtual void Update(float deltaTime) override;
		virtual void Render();
		virtual void EndPlay() override;

		void SetMesh(MeshAsset* meshAsset);

	protected:
		RenderMesh* renderMesh = nullptr;
		MeshAsset* meshAsset = nullptr;
	};
}