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

	void MeshComponent::Render()
	{
		if (renderMesh)
			renderMesh->Render(parent);
	}

	void MeshComponent::EndPlay()
	{
	}
	
	void MeshComponent::SetMesh(Asset _meshAsset)
	{
		this->meshAsset = _meshAsset;
		meshAsset->Load();
		renderMesh.reset(gEngine->GetRenderDevice()->CreateMesh(this->meshAsset));
	}
}
