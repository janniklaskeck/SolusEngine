#include "entity/component/SCameraComponent.hpp"

#include "core/processor/SCameraProcessor.hpp"
#include "core/SEngine.hpp"

namespace Solus
{

	SCameraComponent::SCameraComponent()
	{
		CameraData.FOV_Y = 90.f;
		CameraData.NearPlane = 0.1f;
		CameraData.FarPlane = 1000.f;

		MoveSpeed = 30.f;
		RotationSpeed = 2.f;

		Yaw = 0.f;
		Pitch = 0.f;
		PitchLimit = 89.f;
	}

}
