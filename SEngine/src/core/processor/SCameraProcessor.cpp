#include "core/processor/SCameraProcessor.hpp"

#include "core/processor/SInputProcessor.hpp"
#include "core/processor/SProcessorManager.hpp"
#include "core/SWorld.hpp"
#include "entity/component/SCameraComponent.hpp"
#include "entity/component/STransformComponent.hpp"
#include "render/editor/SEditorInterface.hpp"

namespace Solus
{

	SCameraProcessor::SCameraProcessor()
	{
	}

	void SCameraProcessor::Tick(float DeltaTime)
	{
		auto CameraCompView = gEngine->GetWorld().GetRegistry().view<STransformComponent, SCameraComponent>();

		CameraCompView.each([this, DeltaTime](entt::entity Entity, STransformComponent& TransformComponent, SCameraComponent& CameraComponent)
			{
				TickCamera(DeltaTime, TransformComponent.GetTransform(), CameraComponent);
			});
	}

	int32 SCameraProcessor::GetPriority() const
	{
		return ProcessorPriority::Physics - 1;
	}

	void SCameraProcessor::TickCamera(float DeltaTime, STransform& Transform, SCameraComponent& Camera)
	{
		TickInput(DeltaTime, Transform, Camera);

		const Vec3 Pos = Transform.GetPosition();

		gEngine->ShowDebugMessage(String::Printf("Camera: (%.2f|%.2f|%.2f)", Pos.x, Pos.y, Pos.z), 0.f);
		gEngine->ShowDebugMessage(String::Printf("Camera: (%.2f|%.2f|%.2f)", Camera.Pitch, Camera.Yaw, 0.f), 0.f);
	}

	void SCameraProcessor::TickInput(float DeltaTime, STransform& Transform, SCameraComponent& Camera)
	{
		if (SEditorInterface::IsMouseOverInterface())
		{
			return;
		}

		const SInputProcessor& InputManager = gEngine->GetProcessorManager().Get<SInputProcessor>();

		auto& Registry = gEngine->GetWorld().GetRegistry();

		const Vec3 CameraForward = Transform.GetForwardVector();
		const Vec3 CameraRight = Transform.GetRightVector();
		const Vec3 CameraUp = Transform.GetUpVector();

		const float Speed = Camera.MoveSpeed * DeltaTime;

		Vec3 PositionDelta{};

		if (InputManager.IsKeyDown(SDLK_W))
		{
			PositionDelta += CameraForward * Speed;
		}

		if (InputManager.IsKeyDown(SDLK_S))
		{
			PositionDelta -= CameraForward * Speed;
		}

		if (InputManager.IsKeyDown(SDLK_A))
		{
			PositionDelta -= CameraRight * Speed;
		}

		if (InputManager.IsKeyDown(SDLK_D))
		{
			PositionDelta += CameraRight * Speed;
		}

		if (InputManager.IsKeyDown(SDLK_LCTRL))
		{
			PositionDelta -= CameraUp * Speed;
		}

		if (InputManager.IsKeyDown(SDLK_SPACE))
		{
			PositionDelta += CameraUp * Speed;
		}

		const Vec2 Delta = InputManager.GetMouseMoveDelta();

		if (glm::length(Delta) > 0.1f)
		{
			Camera.Yaw += Delta.x * DeltaTime * Camera.RotationSpeed;
			Camera.Pitch = std::clamp(Camera.Pitch + Delta.y * DeltaTime * Camera.RotationSpeed, -Camera.PitchLimit, Camera.PitchLimit);
			Transform.SetRotation({ Camera.Pitch, Camera.Yaw, 0.f });
		}

		Transform.AddPosition(PositionDelta);
	}

}
