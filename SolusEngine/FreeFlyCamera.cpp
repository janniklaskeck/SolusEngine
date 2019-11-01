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

		float speed = 0.1f;

		Vec3f prevRot = GetRotation();
		//AddRotation(Vec3f(yDelta * speed, xDelta * speed, 0));
		AddRotation(Vec3f(0, xDelta * speed, 0));
		if (InputDevice::IsKeyDown(GLFW_KEY_W))
		{
			Vec3f curPos = GetPosition();
			Vec3f forward = GetForward();
			SetPosition(curPos + (forward * speed));
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_S))
		{
			Vec3f curPos = GetPosition();
			Vec3f forward = GetForward();
			SetPosition(curPos + (forward * -speed));
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_A))
		{
			Vec3f curPos = GetPosition();
			Vec3f right = GetRight();
			SetPosition(curPos + (right * -speed));
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_D))
		{
			Vec3f curPos = GetPosition();
			Vec3f right = GetRight();
			SetPosition(curPos + (right * speed));
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_SPACE))
		{
			Vec3f curPos = GetPosition();
			Vec3f up = GetUp();
			SetPosition(curPos + (up * speed));
		}
		if (InputDevice::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Vec3f curPos = GetPosition();
			Vec3f up = GetUp();
			SetPosition(curPos + (up * -speed));
		}
		return false;
	}
}
