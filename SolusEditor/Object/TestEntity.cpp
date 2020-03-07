#include "TestEntity.h"

#include "Engine/Engine.h"

namespace Editor
{

	TestEntity::TestEntity()
	{
		meshComponent = new Solus::MeshComponent();
		//meshComponent->Attach(this);
		AttachComponent(meshComponent);
	}

	void TestEntity::BeginPlay()
	{
		Solus::MeshAsset* asset = (Solus::MeshAsset*)Solus::gEngine->GetAssetManager()->GetAsset("Model/suzanne.obj");
		meshComponent->SetMesh(asset);
	}

}
