#include "TestEntity.h"

#include "Engine/Engine.h"

namespace Editor
{

	TestEntity::TestEntity()
	{
		meshComponent.reset(new Solus::MeshComponent());
		AttachComponent(meshComponent.get());
	}

	void TestEntity::BeginPlay()
	{
		//Solus::MeshAsset* asset = (Solus::MeshAsset*)Solus::gEngine->GetAssetManager()->GetAsset("Model/suzanne.obj");
		//meshComponent->SetMesh(asset);
	}

}
