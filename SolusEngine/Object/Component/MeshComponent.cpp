#include "MeshComponent.h"
#include "Render/RenderMesh.h"
#include "Engine/Engine.h"

namespace Solus
{
	SCLASS_IMPL(MeshComponent);

	void MeshComponent::BeginPlay()
	{
	}

	void MeshComponent::Update(float deltaTime)
	{
		
	}

	void MeshComponent::Render()
	{
		renderMesh->Render();
	}

	void MeshComponent::EndPlay()
	{
		
	}
	
	void MeshComponent::SetMesh(MeshAsset* meshAsset)
	{
		if (renderMesh)
			delete renderMesh;
		this->meshAsset = meshAsset;
		renderMesh = gEngine->GetRenderDevice()->CreateMesh(meshAsset);
		renderMesh->owner = parent;
	}
}
