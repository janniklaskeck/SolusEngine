#pragma once

#include "Window.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Solus
{
	class SOLUS_API GLFWWindow : public Window
	{
	public:

		GLFWWindow();
		~GLFWWindow();

		virtual void Initialize() override;
		virtual void Run() override;
		virtual void Update()override;
		virtual void Render() override;

		virtual void PreRenderUI() override;
		virtual void RenderUI() override;

		virtual void Destroy() override;

		virtual void GetCursorPos(double* xPos, double* yPos) override;
		virtual double GetTime() const override;

		virtual Vec2i GetWindowPosition() const override;

		virtual Vec2i GetWindowSize() const override;

		virtual void SetMouseVisible(bool isVisible) override;

		virtual void SetWindowTitle(const std::string title) override;

		GLFWwindow* GetInternalWindow() const;

	protected:

		static void KeyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MousePosCallbackForwarder(GLFWwindow* window, double xPos, double yPos);
		static void MouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallbackForwarder(GLFWwindow* window, double xOffset, double yOffset);

	protected:
		GLFWwindow* window = nullptr;

		int width = 1280;
		int height = 720;
		std::string title = "SolusEngine";

	};

}
