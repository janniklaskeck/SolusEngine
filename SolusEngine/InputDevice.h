#pragma once

#include "SolusEngine.h"
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

		static bool IsKeyDown(int keyCode);
		static bool IsKeyUp(int keyCode);

		static bool IsButtonDown(int mouseButton);
		static bool IsButtonUp(int mouseButton);

		static void GetMousePosDelta(double& xDelta, double& yDelta);
		static void GetMouseScrollDelta(double& xDelta, double& yDelta);

	private:
		static std::unordered_map<int, int> keyState;
		static double mouseXPos;
		static double mouseYPos;

		static double previousMouseXPos;
		static double previousMouseYPos;

		static std::unordered_map<int, int> mouseButtonState;
		static double scrollXOffset;
		static double scrollYOffset;
	};
}

