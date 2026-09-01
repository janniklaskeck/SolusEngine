#include "entity/SEntity.hpp"

namespace Solus
{



	SEntity::SEntity(const entt::entity InEntityHandle, SWorld* InWorld)
		: World(InWorld), EntityHandle(InEntityHandle)
	{

	}

}