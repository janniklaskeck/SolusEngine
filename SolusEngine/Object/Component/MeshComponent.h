#pragma once

#include "SComponent.h"

namespace Solus
{
	class RenderMesh;
	class MeshAsset;

	class SOLUS_API MeshComponent : public SComponent
	{
	public:
		virtual void BeginPlay() override;
		virtual void Update(float deltaTime) override;
		virtual void EndPlay() override;

		void SetMesh(MeshAsset* meshAsset);

	protected:
		RenderMesh* renderMesh = nullptr;
		MeshAsset* meshAsset = nullptr;
	};
}