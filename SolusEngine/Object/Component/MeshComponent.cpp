#include "MeshComponent.h"

#include "Render/RenderMesh.h"

#include "Engine/Engine.h"

#include "AssetSystem/MeshAsset.h"
#include "AssetSystem/TextureAsset.h"

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
		renderMesh.reset(gEngine->GetRenderDevice()->CreateMesh(*meshAsset.GetAs<MeshAsset>()));
		textureAssets.resize(meshAsset.GetAs<MeshAsset>()->GetMesh().textureCount);
	}

	void MeshComponent::SetTexture(uint8_t index, Asset textureAsset)
	{
		if (index < 0 || index >= textureAssets.size())
			return;
		textureAssets[index] = textureAsset;
		renderMesh->SetTexture(index, *textureAsset.GetAs<TextureAsset>());
	}

}
