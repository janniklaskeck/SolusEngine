#include "TestEntity.h"

#include "Engine/Engine.h"

namespace Solus
{

	TestEntity::TestEntity()
	{
		meshComponent.reset(new Solus::MeshComponent());
		AttachComponent(meshComponent.get());
	}

	void TestEntity::BeginPlay()
	{
		//Solus::MeshAsset* asset = (Solus::MeshAsset*)Solus::gEngine->GetAssetManager()->GetAssetFromPath("Model/suzanne.obj");
		//meshComponent->SetMesh(asset);
	}

}
