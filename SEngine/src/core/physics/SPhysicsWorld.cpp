#include "core/physics/SPhysicsWorld.hpp"

#include "entity/component/SRigidBodyComponent.hpp"

#include "Jolt/RegisterTypes.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "../../../include/entity/component/STransformComponent.hpp"

namespace Solus
{
	JPH_SUPPRESS_WARNINGS

	// Callback for traces, connect this to your own trace function if you have one
	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		// Print to the TTY
		std::cout << buffer << std::endl;
	}

#ifdef JPH_ENABLE_ASSERTS

	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

		// Breakpoint
		return true;
	};

#endif // JPH_ENABLE_ASSERTS

	SPhysicsWorld::SPhysicsWorld()
	{
		JPH::RegisterDefaultAllocator();

		{
			// Install trace and assert callbacks
			JPH::Trace = TraceImpl;
			JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)
		}

		// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
		// It is not directly used in this example but still required.
		JPH::Factory::sInstance = new JPH::Factory();

		// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
		// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
		// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
		JPH::RegisterTypes();

		JobSystem.Init(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

		PhysicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

		PhysicsSystem.SetGravity({ 0.f, -9.81f, 0.f });

		constexpr int32 TempAllocatorSize = 256 * 1024 * 1024;
		TempAllocator = std::make_unique<JPH::TempAllocatorImpl>(TempAllocatorSize);

		// A body activation listener gets notified when bodies activate and go to sleep
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		PhysicsSystem.SetBodyActivationListener(&body_activation_listener);

		// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		PhysicsSystem.SetContactListener(&contact_listener);
	}

	SPhysicsWorld::~SPhysicsWorld()
	{
		// Unregisters all types with the factory and cleans up the default material
		JPH::UnregisterTypes();

		// Destroy the factory
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}

	void SPhysicsWorld::Tick(float DeltaTime)
	{
		// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
		// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
		// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
		//PhysicsSystem.OptimizeBroadPhase();
	
		// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
		constexpr float PhysicsDeltaTime = 1.0f / 60.0f;

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int CollisionSteps = std::min(6, std::max(1, (int)std::ceil(DeltaTime / PhysicsDeltaTime)));
		PhysicsSystem.Update(PhysicsDeltaTime, CollisionSteps, TempAllocator.get(), &JobSystem);
	}

	STransform SPhysicsWorld::GetBodyTransform(const JPH::BodyID BodyId) const
	{
		const JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();

		const JPH::Quat Rotation = BodyInterface.GetRotation(BodyId);
		const JPH::Vec4 Quat = Rotation.GetXYZW();
		const JPH::RVec3 Position = BodyInterface.GetPosition(BodyId);

		STransform BodyTransform;
		BodyTransform.SetPosition({ Position.GetX(), Position.GetY(), Position.GetZ() });
		BodyTransform.SetRotation({ Quat.GetW(), Quat.GetX(), Quat.GetY(), Quat.GetZ() });

		return BodyTransform;
	}

	void SPhysicsWorld::SetActive(const SRigidBodyComponent& Component, const bool bActive)
	{
		JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();
		if (bActive)
		{
			BodyInterface.ActivateBody(Component.BodyId);
		}
		else
		{
			BodyInterface.DeactivateBody(Component.BodyId);
		}
	}

	const JPH::BodyID SPhysicsWorld::CreateBoxBody(SRigidBodyComponent& RigidBodyComponent)
	{
		// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
		// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
		JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();

		// Next we can create a rigid body to serve as the floor, we make a large box
		// Create the settings for the collision volume (the shape).
		// Note that for simple shapes (like boxes) you can also directly construct a BoxShape.

		const Vec3 Extents = RigidBodyComponent.BoxExtents;
		JPH::BoxShapeSettings BoxShapeSettings(JPH::Vec3(Extents.x, Extents.y, Extents.z));
		BoxShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

		// Create the shape
		JPH::ShapeSettings::ShapeResult BoxShapeResult = BoxShapeSettings.Create();
		JPH::ShapeRefC BoxShapeRef = BoxShapeResult.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

		JPH::EMotionType MotionType = RigidBodyComponent.MotionType;

		JPH::ObjectLayer ObjectLayer = MotionType != JPH::EMotionType::Static ? Layers::MOVING : Layers::NON_MOVING;

		const STransform& Transform = RigidBodyComponent.Owner.GetComponent<STransformComponent>().GetTransform();

		// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
		JPH::Quat Rotation;
		Rotation.SetW(Transform.GetRotation().w);
		Rotation.SetX(Transform.GetRotation().x);
		Rotation.SetY(Transform.GetRotation().y);
		Rotation.SetZ(Transform.GetRotation().z);
		JPH::BodyCreationSettings BodyCreationSettings(BoxShapeRef, JPH::RVec3(Transform.GetPosition().x, Transform.GetPosition().y, Transform.GetPosition().z), Rotation, MotionType, ObjectLayer);

		// Create the actual rigid body
		//JPH::Body* floor = body_interface.CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr
		JPH::Body* Body = BodyInterface.CreateBody(BodyCreationSettings);
		JPH::BodyID BodyId = Body->GetID();
	

		return BodyId;
	}

	void SPhysicsWorld::QueueBodyCreation(SRigidBodyComponent& RigidBodyComponent)
	{
		RigidBodyComponent.BodyId = CreateBoxBody(RigidBodyComponent);

		if (RigidBodyComponent.MotionType != JPH::EMotionType::Static && RigidBodyComponent.bStartActivatedWhenNotStatic)
		{
			ActivatedConstructionQueue.push_back(RigidBodyComponent.BodyId);
		}
		else
		{
			DormantConstructionQueue.push_back(RigidBodyComponent.BodyId);
		}
	}

	void SPhysicsWorld::QueueBodyDestruction(SRigidBodyComponent& RigidBodyComponent)
	{
		BodyDestructionQueue.push_back(RigidBodyComponent.BodyId);
	}

	void SPhysicsWorld::FlushQueues()
	{
		JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();

		JPH::BodyInterface::AddState ActivatedState = BodyInterface.AddBodiesPrepare(ActivatedConstructionQueue.data(), ActivatedConstructionQueue.size());
		BodyInterface.AddBodiesFinalize(ActivatedConstructionQueue.data(), ActivatedConstructionQueue.size(), ActivatedState, JPH::EActivation::Activate);

		JPH::BodyInterface::AddState DormantState = BodyInterface.AddBodiesPrepare(DormantConstructionQueue.data(), DormantConstructionQueue.size());
		BodyInterface.AddBodiesFinalize(DormantConstructionQueue.data(), DormantConstructionQueue.size(), DormantState, JPH::EActivation::DontActivate);

		BodyInterface.RemoveBodies(BodyDestructionQueue.data(), BodyDestructionQueue.size());
		BodyInterface.DestroyBodies(BodyDestructionQueue.data(), BodyDestructionQueue.size());

		ActivatedConstructionQueue.clear();
		DormantConstructionQueue.clear();
		BodyDestructionQueue.clear();
	}

	void SPhysicsWorld::RemovePhysicsBody(const JPH::BodyID BodyId)
	{
		JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();
		BodyInterface.RemoveBody(BodyId);
	}

	void SPhysicsWorld::DestroyPhysicsBody(const JPH::BodyID BodyId)
	{
		JPH::BodyInterface& BodyInterface = PhysicsSystem.GetBodyInterface();
		BodyInterface.DestroyBody(BodyId);
	}

	BPLayerInterfaceImpl::BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	JPH::uint BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

	const char* BPLayerInterfaceImpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}

	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

	JPH::ValidateResult MyContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
	{
		//std::cout << "Contact validate callback" << std::endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	void MyContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		//std::cout << "A contact was added" << std::endl;
	}

	void MyContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		//std::cout << "A contact was persisted" << std::endl;
	}

	void MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
	{
		//std::cout << "A contact was removed" << std::endl;
	}

	void MyBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData)
	{
		//std::cout << "A body got activated" << std::endl;
	}

	void MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData)
	{
		//std::cout << "A body went to sleep" << std::endl;
	}

	bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

}