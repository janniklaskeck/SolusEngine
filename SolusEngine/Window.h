#pragma once

#include "SolusEngine.h"
#include "InputDevice.h"

#include <GLFW/glfw3.h>

namespace Solus
{

	class SOLUS_API Window
	{
	public:
		Window(InputDevice* inputDevice);
		~Window();

		virtual void Run();

		virtual void Close();

		InputDevice* GetInputDevice() const
		{
			return inputDevice;
		}

		GLFWwindow* GetGLFWWindow() const
		{
			return window;
		}

	protected:
		virtual void Update() = 0;

		static void KeyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MousePosCallbackForwarder(GLFWwindow* window, double xPos, double yPos);
		static void MouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallbackForwarder(GLFWwindow* window, double xOffset, double yOffset);
	private:
		GLFWwindow* window;
		InputDevice* inputDevice;
	};
}

