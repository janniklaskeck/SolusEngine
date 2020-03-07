#pragma once

#include "Object/Entity.h"
#include "Object/Component/MeshComponent.h"
#include "AssetSystem/MeshAsset.h"

namespace Editor
{
	class TestEntity : public Solus::Entity
	{
	public:
		TestEntity();
		virtual void BeginPlay() override;

	private:
		Solus::MeshComponent* meshComponent;
	};
}
