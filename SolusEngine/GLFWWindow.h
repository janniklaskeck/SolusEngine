#pragma once

#include "RenderWindow.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Solus
{

	class GLFWWindow : public RenderWindow
	{
	public:

		GLFWWindow();
		~GLFWWindow();

		virtual void Initialize() override;
		virtual void Run() override;
		virtual void Update()override;
		virtual void Close() override;

		virtual void GetCursorPos(double* xPos, double* yPos) override;
		virtual double GetTime() const override;
		virtual void GetWindowSize(int* xSize, int* ySize) override;

	protected:

		static void KeyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MousePosCallbackForwarder(GLFWwindow* window, double xPos, double yPos);
		static void MouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallbackForwarder(GLFWwindow* window, double xOffset, double yOffset);

	private:
		GLFWwindow* window;
	};

}
