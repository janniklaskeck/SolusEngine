#pragma once

#include "Object/Entity.h"
#include "Object/Component/MeshComponent.h"
#include "AssetSystem/MeshAsset.h"

namespace Editor
{
	class TestEntity : public Solus::Entity
	{
	public:
		TestEntity(Solus::MeshAsset* mesh);

	private:
		Solus::MeshComponent* meshComponent;
	};
}
