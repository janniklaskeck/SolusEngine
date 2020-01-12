#include "InputDevice.h"
#include "Engine/Engine.h"
#include "Render/Window.h"

#include <GLFW/glfw3.h>

namespace Solus
{
	InputDevice::InputDevice()
		: previousMouseXPos(-1.),
		previousMouseYPos(-1.),
		mouseXPos(0),
		mouseYPos(0),
		scrollXOffset(0),
		scrollYOffset(0)
	{	}

	InputDevice::~InputDevice()
	{}

	void InputDevice::Initialize()
	{}

	void InputDevice::Update()
	{
		double xPos, yPos;
		gEngine->GetWindow()->GetCursorPos(&xPos, &yPos);
		previousMouseXPos = mouseXPos;
		previousMouseYPos = mouseYPos;
		mouseXPos = xPos;
		mouseYPos = yPos;
	}

	void InputDevice::Destroy()
	{}

	bool InputDevice::IsKeyDown(int keyCode)
	{
		return keyState[keyCode] == GLFW_PRESS || keyState[keyCode] == GLFW_REPEAT;
	}

	bool InputDevice::IsKeyUp(int keyCode)
	{
		return keyState[keyCode] == GLFW_RELEASE;
	}

	bool InputDevice::IsButtonDown(int mouseButton)
	{
		return mouseButtonState[mouseButton] == GLFW_PRESS || mouseButtonState[mouseButton] == GLFW_REPEAT;
	}

	bool InputDevice::IsButtonUp(int mouseButton)
	{
		return mouseButtonState[mouseButton] == GLFW_RELEASE;
	}

	void InputDevice::GetMousePosDelta(float& xDelta, float& yDelta)
	{
		xDelta = previousMouseXPos - mouseXPos;
		yDelta = previousMouseYPos - mouseYPos;
	}

	void InputDevice::GetMouseScrollDelta(float& xDelta, float& yDelta)
	{
		xDelta = scrollXOffset;
		yDelta = scrollYOffset;
		scrollXOffset = 0.0;
		scrollYOffset = 0.0;
	}

	void InputDevice::KeyEventUpdate(int key, int scancode, int action, int mods)
	{
		keyState[key] = action;
	}

	void InputDevice::MousePosEventUpdate(double xPos, double yPos)
	{}

	void InputDevice::MouseButtonEventUpdate(int key, int action, int mods)
	{
		mouseButtonState[key] = action;
	}

	void InputDevice::MouseScrollEventUpdate(double xOffset, double yOffset)
	{
		scrollXOffset = xOffset;
		scrollYOffset = yOffset;
	}
}
