#include "TestEntity.h"

namespace Editor
{

	TestEntity::TestEntity(Solus::MeshAsset* mesh)
	{
		meshComponent = new Solus::MeshComponent();
		meshComponent->Attach(this);
		meshComponent->SetMesh(mesh);

	}

}
