#include "Object/Component/SComponent.h"

#include "Object/Entity.h"

namespace Solus
{
	void SComponent::Attach(Entity* parent)
	{
		this->parent = parent;
	}
}