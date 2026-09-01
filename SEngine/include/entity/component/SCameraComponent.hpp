#pragma once

#include "SEntityComponent.hpp"

#include <bgfx/bgfx.h>
#include <array>
#include <entt/entt.hpp>

namespace Solus
{


	class SCameraComponent : public SEntityComponent
	{
	public:

		SCameraComponent();

		static void on_construct(entt::registry& Registry, const entt::entity Entity);

	public:
		float FOV_Y = 0.f;

		float NearPlane = 0.f;
		float FarPlane = 0.f;

		float MoveSpeed = 30.f;
		float RotationSpeed = 2.f;

		float Yaw = 0.f;
		float Pitch = 0.f;

		float PitchLimit = 89.f;

		std::array<float, 16> ProjectionMatrix;

		bgfx::ViewId ViewId;
	};

}