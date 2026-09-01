#pragma once

#include "SEntityComponent.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/MotionType.h>

namespace Solus
{

	class SRigidBodyComponent : public SEntityComponent
	{
	public:

		static void on_destroy(entt::registry& Registry, const entt::entity Entity);

		void ConstructBody();

	public:

		Vec3 BoxExtents{};

		bool bOverrideMass = false;
		float MassOverride = 1.f;

		JPH::EMotionType MotionType = JPH::EMotionType::Static;
		bool bStartActivatedWhenNotStatic = true;

		float Friction = 0.5f;
		float Restitution = 0.2f;

		float LinearDampening = 0.3f;
		float AngularDampening = 0.f;

		JPH::BodyID BodyId;
	};
}