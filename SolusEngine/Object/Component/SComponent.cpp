#include "Object/Component/SComponent.h"

#include "Object/Entity.h"

#include "Engine/Engine.h"

namespace Solus
{
	SCLASS_IMPL(SComponent);

	void SComponent::BeginPlay()
	{
		hasBegunPlay = true;
	}

	void SComponent::Update(float deltaTime)
	{}

	void SComponent::EndPlay()
	{}

	void SComponent::Attach(Entity* parent)
	{
		
		this->parent = parent;
	}
}