#pragma once

#include "SEntityComponent.hpp"
#include "render/camera/SCamera.hpp"

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>

namespace Solus
{


	class SCameraComponent : public SEntityComponent
	{
	public:

		SCameraComponent();

	public:
		
		SCameraData CameraData;

		float MoveSpeed = 30.f;
		float RotationSpeed = 2.f;

		float Yaw = 0.f;
		float Pitch = 0.f;

		float PitchLimit = 89.f;
	};

}