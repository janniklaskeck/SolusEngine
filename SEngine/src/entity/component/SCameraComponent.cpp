#include "entity/component/SCameraComponent.hpp"
#include "../../../include/core/processor/SCameraProcessor.hpp"
#include "../../../include/core/SEngine.hpp"

namespace Solus
{

	SCameraComponent::SCameraComponent()
	{
		FOV_Y = 90.f;

		NearPlane = 0.1f;
		FarPlane = 1000.f;

		MoveSpeed = 30.f;
		RotationSpeed = 2.f;
		PitchLimit = 89.f;

		Yaw = 0.f;
		Pitch = 0.f;

		ViewId = 0;
	}

	void SCameraComponent::on_construct(entt::registry& Registry, const entt::entity Entity)
	{
		SCameraProcessor& CameraProcessor = gEngine->GetProcessor<SCameraProcessor>();
		CameraProcessor.RegisterCameraComponent(Registry.get<SCameraComponent>(Entity));
	}

}
