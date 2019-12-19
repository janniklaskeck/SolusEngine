#pragma once

#include "Engine/SolusEngine.h"
#include "InputListener.h"

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Solus
{
	class SOLUS_API InputDevice
	{
	public:
		InputDevice();
		virtual ~InputDevice();

		virtual void Update();

		virtual void KeyEventUpdate(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void MousePosEventUpdate(GLFWwindow* window, double xPos, double yPos);
		virtual void MouseButtonEventUpdate(GLFWwindow* window, int key, int action, int mods);
		virtual void MouseScrollEventUpdate(GLFWwindow* window, double xOffset, double yOffset);

		bool IsKeyDown(int keyCode);
		bool IsKeyUp(int keyCode);

		bool IsButtonDown(int mouseButton);
		bool IsButtonUp(int mouseButton);

		void GetMousePosDelta(double& xDelta, double& yDelta);
		void GetMouseScrollDelta(double& xDelta, double& yDelta);

	private:
		std::unordered_map<int, int> keyState;
		double mouseXPos;
		double mouseYPos;

		double previousMouseXPos;
		double previousMouseYPos;

		std::unordered_map<int, int> mouseButtonState;
		double scrollXOffset;
		double scrollYOffset;
	};
}

