#include "core/physics/SPhysicsBody.hpp"

#include "core/physics/SPhysicsWorld.hpp"

namespace Solus
{
	SPhysicsBody::SPhysicsBody()
	{
	}

	SPhysicsBody::~SPhysicsBody()
	{

	}

	void SPhysicsBody::Init(SPhysicsWorld* InParentWorld)
	{
		ParentWorld = InParentWorld;
	}

	void SPhysicsBody::CreateBoxBody(const STransform& Transform, const Vec3 Extents, const bool bDynamic)
	{
		
	}

}
