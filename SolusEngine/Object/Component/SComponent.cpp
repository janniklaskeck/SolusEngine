#include "Object/Component/SComponent.h"

#include "Object/Entity.h"

namespace Solus
{
	SCLASS_IMPL(SComponent);

	void SComponent::Attach(Entity* parent)
	{
		this->parent = parent;
		BeginPlay();
	}
}