#include "entity/SEntity.hpp"

namespace Solus
{



	SEntity::SEntity(const entt::handle InEntityHandle, SWorld* InWorld)
		: World(InWorld), EntityHandle(InEntityHandle)
	{

	}

	void SEntity::Destroy()
	{
		EntityHandle.erase<entt::entity>();
	}

}