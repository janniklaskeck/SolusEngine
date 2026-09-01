#include "entity/component/SRigidBodyComponent.hpp"

#include "core/physics/SPhysicsWorld.hpp"
#include "core/SEngine.hpp"

namespace Solus
{
	void SRigidBodyComponent::on_destroy(entt::registry& Registry, const entt::entity Entity)
	{
		// queue body destruction
		SRigidBodyComponent& RigidBodyComponent = Registry.get<SRigidBodyComponent>(Entity);
		SPhysicsWorld& ParentWorld = gEngine->GetPhysicsWorld();
		ParentWorld.QueueBodyDestruction(RigidBodyComponent);
	}

	void SRigidBodyComponent::ConstructBody()
	{
		SPhysicsWorld& ParentWorld = gEngine->GetPhysicsWorld();
		ParentWorld.QueueBodyCreation(*this);
	}
}
