#include "MeshComponent.h"

#include "Render/RenderMesh.h"

#include "Engine/Engine.h"

#include "AssetSystem/MeshAsset.h"
#include "AssetSystem/TextureAsset.h"

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
					const MeshAsset* meshAssetPtr = meshAsset.GetAs<MeshAsset>();
					renderMesh = meshAssetPtr->GetRenderMesh();
					textureAssets.clear();
					textureAssets.resize(meshAssetPtr->GetMesh().textureCount);
				}
			}
			if (renderMesh)
			{
				for (int i = 0; i < textureAssets.size(); i++)
				{
					if (textureAssets[i])
						renderMesh->SetTexture(i, textureAssets[i].GetAs<TextureAsset>());
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
