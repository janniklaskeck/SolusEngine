#include "MeshComponent.h"
#include "Render/RenderMesh.h"
#include "Engine/Engine.h"

namespace Solus
{
	void MeshComponent::BeginPlay()
	{
		
	}

	void MeshComponent::Update(float deltaTime)
	{
		
	}

	void MeshComponent::EndPlay()
	{
		
	}
	
	void MeshComponent::SetMesh(MeshAsset* meshAsset)
	{
		this->meshAsset = meshAsset;
		this->renderMesh = gEngine->GetRenderDevice()->CreateMesh(meshAsset);
	}
}
