#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/BodyID.h>


namespace Solus
{
	class SPhysicsWorld;

	class SPhysicsBody
	{
		friend class SPhysicsWorld;

	public:

		SPhysicsBody();

		~SPhysicsBody();

	public:

		void Init(SPhysicsWorld* InParentWorld);

		void CreateBox(const Vec3 Extents);

		void CreateBoxBody(const STransform& Transform, const Vec3 Extents, const bool bDynamic);
		
	private:

		JPH::BodyID BodyId;

		SPhysicsWorld* ParentWorld = nullptr;
	};
}