#include "InputDevice.h"
#include "Engine.h"
#include "RenderWindow.h"

namespace Solus
{
	std::unordered_map<int, int> InputDevice::keyState = std::unordered_map<int, int>();
	std::unordered_map<int, int> InputDevice::mouseButtonState = std::unordered_map<int, int>();

	double InputDevice::previousMouseXPos = -1.;
	double InputDevice::previousMouseYPos = -1.;
	double InputDevice::mouseXPos = .0;
	double InputDevice::mouseYPos = .0;
	double InputDevice::scrollXOffset = .0;
	double InputDevice::scrollYOffset = .0;

	InputDevice::InputDevice()
	{}

	InputDevice::~InputDevice()
	{}

	void InputDevice::Update()
	{
		double xPos, yPos;
		gEngine->GetRenderWindow()->GetCursorPos(&xPos, &yPos);
		previousMouseXPos = mouseXPos;
		previousMouseYPos = mouseYPos;
		mouseXPos = xPos;
		mouseYPos = yPos;
	}

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

	void InputDevice::GetMousePosDelta(double& xDelta, double& yDelta)
	{
		xDelta = previousMouseXPos - mouseXPos;
		yDelta = previousMouseYPos - mouseYPos;
	}

	void InputDevice::GetMouseScrollDelta(double& xDelta, double& yDelta)
	{
		xDelta = scrollXOffset;
		yDelta = scrollYOffset;
	}

	void InputDevice::KeyEventUpdate(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyState[key] = action;
	}

	void InputDevice::MousePosEventUpdate(GLFWwindow* window, double xPos, double yPos)
	{
	}

	void InputDevice::MouseButtonEventUpdate(GLFWwindow* window, int key, int action, int mods)
	{
		mouseButtonState[key] = action;
	}

	void InputDevice::MouseScrollEventUpdate(GLFWwindow* window, double xOffset, double yOffset)
	{
		scrollXOffset = xOffset;
		scrollYOffset = yOffset;
	}
}
