#include "FreeFlyCamera.h"
#include "Input/InputDevice.h"

#include "Engine/Engine.h"

#include <GLFW/glfw3.h>

namespace Solus
{
	SCLASS_IMPL(FreeFlyCamera);

	Mat4f* FreeFlyCamera::GetViewMatrix()
	{
		return &mTransform;
	}

	bool FreeFlyCamera::Update()
	{
		Entity::Update();

		gEngine->GetWindow()->SetMouseVisible(!IsInputEnabled());
		if (!IsInputEnabled())
			return true;
		auto* input = gEngine->GetInputDevice();

		float xDelta, yDelta;
		input->GetMousePosDelta(xDelta, yDelta);
		float deltaTime = (float)gEngine->DeltaTime();
		float speed = movementSpeed * deltaTime;
		float rotationSpeed = 0.1f * deltaTime;
		AddRotation(Vec3f(yDelta, xDelta, 0.0f) * rotationSpeed);

		float xScroll, yScroll;
		input->GetMouseScrollDelta(xScroll, yScroll);

		if (std::abs(yScroll) > 0.1)
		{
			movementSpeed = std::clamp<float>(movementSpeed + yScroll, 1.0, 20.0);
		}

		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
			speed *= 2.0f;
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_W))
		{
			Vec3f forward = GetForward();
			AddPosition(forward * speed);
		}
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_S))
		{
			Vec3f forward = GetForward();
			AddPosition(-forward * speed);
		}
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_A))
		{
			Vec3f right = GetRight();
			AddPosition(-right * speed);
		}
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_D))
		{
			Vec3f right = GetRight();
			AddPosition(right * speed);
		}
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_SPACE))
		{
			Vec3f up = GetUp();
			AddPosition(up * speed);
		}
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Vec3f up = GetUp();
			AddPosition(-up * speed);
		}
		return false;
	}
}
