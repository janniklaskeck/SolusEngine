#include "core/processor/SCameraProcessor.hpp"

#include "core/SEngine.hpp"
#include "entity/component/STransformComponent.hpp"
#include "core/SWorld.hpp"
#include "core/processor/SProcessorManager.hpp"
#include "core/processor/SInputProcessor.hpp"

#include <bgfx/bgfx.h>
#include "../../../include/entity/component/SCameraComponent.hpp"
#include "core/SEngine.hpp"
#include "core/SWorld.hpp"
#include "render/SWindow.hpp"

#include "bx/math.h"
#include "bgfx/bgfx.h"

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
				TickCamera(DeltaTime, TransformComponent, CameraComponent);
			});
	}

	int32 SCameraProcessor::GetPriority() const
	{
		return ProcessorPriority::Physics - 1;
	}

	void SCameraProcessor::RegisterCameraComponent(SCameraComponent& CameraComponent)
	{
		SetupProjectionMatrix(CameraComponent);
	}

	void SCameraProcessor::TickCamera(float DeltaTime, STransformComponent& TransformComponent, SCameraComponent& CameraComponent)
	{
		TickInput(DeltaTime, TransformComponent, CameraComponent);

		SetViewProjectionMatrices(TransformComponent, CameraComponent);

		const STransform& Transform = TransformComponent.GetTransform();
		const Vec3 Pos = Transform.GetPosition();

		gEngine->ShowDebugMessage(String::Printf("Camera: (%.2f|%.2f|%.2f)", Pos.x, Pos.y, Pos.z), 0.f);
		gEngine->ShowDebugMessage(String::Printf("Camera: (%.2f|%.2f|%.2f)", CameraComponent.Pitch, CameraComponent.Yaw, 0.f), 0.f);
	}

	void SCameraProcessor::TickInput(float DeltaTime, STransformComponent& TransformComponent, SCameraComponent& CameraComponent)
	{
		const SInputProcessor& InputManager = gEngine->GetProcessorManager().Get<SInputProcessor>();

		auto& Registry = gEngine->GetWorld().GetRegistry();

		STransform& Transform = TransformComponent.GetTransform();

		const Vec3 CameraForward = Transform.GetForwardVector();
		const Vec3 CameraRight = Transform.GetRightVector();
		const Vec3 CameraUp = Transform.GetUpVector();

		const float Speed = CameraComponent.MoveSpeed * DeltaTime;

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
			CameraComponent.Yaw += Delta.x * DeltaTime * CameraComponent.RotationSpeed;
			CameraComponent.Pitch = std::clamp(CameraComponent.Pitch + Delta.y * DeltaTime * CameraComponent.RotationSpeed, -CameraComponent.PitchLimit, CameraComponent.PitchLimit);
			Transform.SetRotation({ CameraComponent.Pitch, CameraComponent.Yaw, 0.f });
		}

		Transform.AddPosition(PositionDelta);
	}

	void SCameraProcessor::SetupProjectionMatrix(SCameraComponent& CameraComponent)
	{
		SWindow* Window = gEngine->GetWindow();

		const int32 WindowWidth = Window->GetWindowWidth();
		const int32 WindowHeight = Window->GetWindowHeight();

		bx::mtxProj(CameraComponent.ProjectionMatrix.data(), CameraComponent.FOV_Y, float(WindowWidth) / float(WindowHeight), CameraComponent.NearPlane, CameraComponent.FarPlane, bgfx::getCaps()->homogeneousDepth);
	}

	void SCameraProcessor::SetViewProjectionMatrices(const STransformComponent& TransformComp, const SCameraComponent& CameraComponent)
	{
		const STransform& Transform = TransformComp.GetTransform();
		const Vec3 CameraPosition = Transform.GetPosition();

		const Vec3 Forward = Transform.GetForwardVector();
		const Vec3 LookAtTarget = CameraPosition + Forward;

		float ViewMatrix[16];

		const bx::Vec3 bxCameraPosition{ CameraPosition.x, CameraPosition.y, CameraPosition.z };
		const bx::Vec3 bxLookAtTarget{ LookAtTarget.x, LookAtTarget.y, LookAtTarget.z };

		bx::mtxLookAt(ViewMatrix, bxCameraPosition, bxLookAtTarget);

		bgfx::setViewTransform(CameraComponent.ViewId, ViewMatrix, CameraComponent.ProjectionMatrix.data());

		SWindow* Window = gEngine->GetWindow();

		const int32 WindowWidth = Window->GetWindowWidth();
		const int32 WindowHeight = Window->GetWindowHeight();

		bgfx::setViewRect(CameraComponent.ViewId, 0, 0, WindowWidth, WindowHeight);
	}

}
