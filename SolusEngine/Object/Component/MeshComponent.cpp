#include "MeshComponent.h"

#include "Render/RenderMesh.h"

#include "Engine/Engine.h"

namespace Solus
{

	void MeshComponent::BeginPlay()
	{}

	void MeshComponent::Update(float deltaTime)
	{
		if (meshAsset)
		{
			if (oldMeshAsset != meshAsset)
			{
				if (oldMeshAsset)
				{
					oldMeshAsset->Unload();
				}
				oldMeshAsset = meshAsset;
				renderMesh = nullptr;
				if (meshAsset)
				{
					meshAsset->Load();
					renderMesh = meshAsset->GetRenderMesh();
					textureAssets.clear();
					textureAssets.resize(meshAsset->GetMesh().textureCount);
				}
			}
			if (renderMesh)
			{
				for (int i = 0; i < textureAssets.size(); i++)
				{
					if (textureAssets[i])
						renderMesh->SetTexture(i, textureAssets[i]);
				}
			}
		}
	}

	void MeshComponent::Render()
	{
		if (renderMesh)
			renderMesh->Render(parent);
	}

	void MeshComponent::EndPlay()
	{}
}
