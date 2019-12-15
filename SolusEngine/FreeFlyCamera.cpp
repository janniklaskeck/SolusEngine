#include "FreeFlyCamera.h"
#include "InputDevice.h"
#include <GLFW/glfw3.h>

namespace Solus
{
	Mat4f* FreeFlyCamera::GetViewMatrix()
	{
		return &mTransform;
	}

	bool FreeFlyCamera::Update()
	{
		Entity::Update();
		double xDelta, yDelta;
		InputDevice::GetMousePosDelta(xDelta, yDelta);
		float deltaTime = gEngine->DeltaTime();
		float speed = 5.f * deltaTime;
		float rotationSpeed = .1f * deltaTime;
		AddRotation(Vec3f(yDelta, xDelta, 0.0f) * rotationSpeed);

		if (InputDevice::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
			speed *= 2.0f;
		if (InputDevice::IsKeyDown(GLFW_KEY_W))
		{
			Vec3f forward = GetForward();
			AddPosition(forward * speed);
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_S))
		{
			Vec3f forward = GetForward();
			AddPosition(-forward * speed);
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_A))
		{
			Vec3f right = GetRight();
			AddPosition(-right * speed);
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_D))
		{
			Vec3f right = GetRight();
			AddPosition(right * speed);
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_SPACE))
		{
			Vec3f up = GetUp();
			AddPosition(up * speed);
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Vec3f up = GetUp();
			AddPosition(-up * speed);
		}
		return false;
	}
}
