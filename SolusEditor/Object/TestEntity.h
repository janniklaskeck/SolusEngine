#pragma once

#include "Object/Entity.h"
#include "Object/Component/MeshComponent.h"
#include "AssetSystem/MeshAsset.h"

namespace Solus
{
	class TestEntity : public Solus::Entity
	{
	public:
		TestEntity();
		virtual void BeginPlay() override;

	private:
		std::unique_ptr<Solus::MeshComponent> meshComponent;
	};
}
