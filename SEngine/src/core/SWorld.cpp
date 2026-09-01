#include "core/SWorld.hpp"

#include "entity/component/STransformComponent.hpp"

namespace Solus
{
	SWorld::SWorld()
	{
	}

	SWorld::~SWorld()
	{

	}

	void SWorld::Tick(float DeltaTime)
	{
		

	}

	SEntity SWorld::CreateEntity(glm::vec3 Position)
	{
		SEntity Entity = SEntity(Registry.create(), this);

		STransformComponent& TransformComp = Entity.AddComponent<STransformComponent>();
		TransformComp.SetTransform({ Position });


		return Entity;
	}

}