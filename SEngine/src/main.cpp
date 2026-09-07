#include "core/SEngine.hpp"

#include "assets/SAssetManager.hpp"
#include "entity/component/STransformComponent.hpp"
#include "entity/component/SRigidBodyComponent.hpp"
#include "entity/component/SMeshComponent.hpp"
#include "entity/component/SCameraComponent.hpp"
#include "core/physics/SPhysicsWorld.hpp"
#include "core/SWorld.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>

using namespace Solus;

int main(int argc, char* argv[])
{
	Solus::SEngine* Engine = new Solus::SEngine;

	Engine->Initialize();

	SWorld& ActiveWorld = gEngine->GetWorld();
	SPhysicsWorld& PhysicsWorld = gEngine->GetPhysicsWorld();

	SEntity CameraEntity = ActiveWorld.CreateEntity({ 0.0f, 0.0f, -35.0f });
	CameraEntity.AddComponent<SCameraComponent>();

	Ref<SShaderProgramAsset> ProgramAsset = SAssetManager::Get().GetAsset<SShaderProgramAsset>();
	ProgramAsset->SetPaths("shaders/vs_mesh.sc", "shaders/fs_mesh.sc");
	Ref<SMeshAsset> MeshAsset = SAssetManager::Get().GetAsset<SMeshAsset>();
	MeshAsset->SetPath("meshes/test.obj");

	{
		SEntity TestFloor = ActiveWorld.CreateEntity({ 0.f, -10.f, 0.f });

		STransformComponent& TransformComp = TestFloor.GetComponent<STransformComponent>();
		TransformComp.GetTransform().SetScale(Vec3(100, 1, 100));

		SMeshComponent& MeshComp = TestFloor.AddComponent<SMeshComponent>();
		MeshComp.MeshAsset = MeshAsset;
		MeshComp.ShaderProgramAsset = ProgramAsset;
		MeshComp.Color = { 1.f, 0.f, 0.f, 1.f };

		SRigidBodyComponent& RBC = TestFloor.AddComponent<SRigidBodyComponent>();
		RBC.BoxExtents = Vec3(100.f, 1.f, 100.f);
		RBC.ConstructBody();
	}

	auto SpawnCube = [&ActiveWorld, &PhysicsWorld, MeshAsset, ProgramAsset](Vec3 Pos)
		{
			SEntity TestCube = ActiveWorld.CreateEntity(Pos);

			STransformComponent& TransformComp = TestCube.GetComponent<STransformComponent>();
			TransformComp.GetTransform().SetRotation({ 40.f, 40.f, 0.f });

			SMeshComponent& MeshComp = TestCube.AddComponent<SMeshComponent>();
			MeshComp.MeshAsset = MeshAsset;
			MeshComp.ShaderProgramAsset = ProgramAsset;
			MeshComp.Color = { 0.f, 1.f, 0.f, 1.f };

			SRigidBodyComponent& RBC = TestCube.AddComponent<SRigidBodyComponent>();
			RBC.MotionType = JPH::EMotionType::Dynamic;
			RBC.BoxExtents = Vec3(1.f, 1.f, 1.f);
			RBC.ConstructBody();
		};



	for (int32 i = 0; i < 100; i++)
	{
		const float min = -25.f;
		const float max = 25.f;
		float x = min + static_cast <float>(std::rand()) / (static_cast <float>(RAND_MAX / (max - min)));
		float y = 10.f;
		float z = min + static_cast <float>(std::rand()) / (static_cast <float>(RAND_MAX / (max - min)));

		SpawnCube({x,y,z});
	}


	while (Engine->Tick())
	{

	}

	Engine->Shutdown();

	return 0;
}