#include "GLFWWindow.h"
#include "Engine/Engine.h"
#include "Input/InputDevice.h"

#include <cassert>

namespace Solus
{
	GLFWWindow::GLFWWindow()
	{

	}

	GLFWWindow::~GLFWWindow()
	{
		glfwTerminate();
	}

	void GLFWWindow::Initialize()
	{
		if (!glfwInit())
		{
			gEngine->Log(LogError, "Could not load glfw!");
			assert(false);
		}
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		window = glfwCreateWindow(1280, 720, "SolusEngine Editor", nullptr, nullptr);

		if (!window)
		{
			glfwTerminate();
			gEngine->Log(LogError, "Could not create glfw Window!");
			assert(false);
		}

		glfwSetKeyCallback(window, KeyCallbackForwarder);
		glfwSetCursorPosCallback(window, MousePosCallbackForwarder);
		glfwSetMouseButtonCallback(window, MouseButtonCallbackForwarder);
		glfwSetScrollCallback(window, MouseScrollCallbackForwarder);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSwapInterval(1);
		glfwMakeContextCurrent(window);

		if (gl3wInit())
		{
			fprintf(stderr, "failed to initialize OpenGL\n");
		}
		if (!gl3wIsSupported(3, 2))
		{
			fprintf(stderr, "OpenGL 3.2 not supported\n");
		}
		printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	void GLFWWindow::Run()
	{
		while (!glfwWindowShouldClose(window))
		{
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			gEngine->Update();

			gEngine->Render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void GLFWWindow::Update()
	{}

	void GLFWWindow::PreRenderUI()
	{
		
	}

	void GLFWWindow::RenderUI()
	{
		
	}

	void GLFWWindow::Destroy()
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void GLFWWindow::GetCursorPos(double* xPos, double* yPos)
	{
		glfwGetCursorPos(window, xPos, yPos);
	}

	double GLFWWindow::GetTime() const
	{
		return glfwGetTime();
	}

	void GLFWWindow::GetWindowSize(int* width, int* height)
	{
		glfwGetFramebufferSize(window, width, height);
	}

	GLFWwindow* GLFWWindow::GetInternalWindow() const
	{
		return window;
	}

	void GLFWWindow::KeyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		gEngine->GetInputDevice()->KeyEventUpdate(key, scancode, action, mods);
	}

	void GLFWWindow::MousePosCallbackForwarder(GLFWwindow* window, double xPos, double yPos)
	{
		gEngine->GetInputDevice()->MousePosEventUpdate(xPos, yPos);
	}

	void GLFWWindow::MouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods)
	{
		gEngine->GetInputDevice()->MouseButtonEventUpdate(button, action, mods);
	}

	void GLFWWindow::MouseScrollCallbackForwarder(GLFWwindow* window, double xOffset, double yOffset)
	{
		gEngine->GetInputDevice()->MouseScrollEventUpdate(xOffset, yOffset);
	}
}
