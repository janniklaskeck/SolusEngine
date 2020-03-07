#include "SolusObject.h"

#include "Object/Entity.h"

namespace Solus
{
	SCLASS_IMPL(SolusObject);

	void SolusObject::BeginPlay()
	{
		hasBegunPlay = true;
	}

	void SolusObject::EndPlay()
	{

	}

	void SolusObject::Update(float deltaTime)
	{

	}
}
