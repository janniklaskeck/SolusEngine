#include "core/processor/SPhysicsProcessor.hpp"

#include "core/processor/SProcessorManager.hpp"
#include "core/SEngine.hpp"
#include "core/physics/SPhysicsWorld.hpp"
#include "entity/component/STransformComponent.hpp"
#include "entity/component/SRigidBodyComponent.hpp"
#include "core/SWorld.hpp"


namespace Solus
{
	void SPhysicsProcessor::Tick(float DeltaTime)
	{
		SPhysicsWorld& PhysicsWorld = gEngine->GetPhysicsWorld();

		PhysicsWorld.FlushQueues();

		PhysicsWorld.Tick(DeltaTime);
		
		auto ComponentView = gEngine->GetWorld().GetRegistry().view<STransformComponent, SRigidBodyComponent>();

		ComponentView.each([&PhysicsWorld](const entt::entity Entity, STransformComponent& TransformComp, SRigidBodyComponent& RigidBodyComp)
			{
				STransform BodyTransform = PhysicsWorld.GetBodyTransform(RigidBodyComp.BodyId);
				TransformComp.GetTransform().SetPosition(BodyTransform.GetPosition());
				TransformComp.GetTransform().SetRotation(BodyTransform.GetRotation());
			});
	}

	int32 SPhysicsProcessor::GetPriority() const
	{
		return ProcessorPriority::Physics;
	}

}