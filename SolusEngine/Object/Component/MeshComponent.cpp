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
		if (meshAsset && meshAsset->GetRenderMesh())
			meshAsset->GetRenderMesh()->Render(parent);
	}

	void MeshComponent::EndPlay()
	{
	}
	
	void MeshComponent::SetMesh(MeshAsset* meshAsset)
	{
		this->meshAsset = meshAsset;
		meshAsset->Load();
	}
}
