#include "Window.h"
#include "Engine.h"

#include <GL/gl3w.h>
#include <IL/devil_cpp_wrapper.hpp>
#include <cassert>

#define SHOW_STATS 0

namespace Solus
{

	Window::Window(InputDevice* inputDevice)
	{
		if (!glfwInit())
		{
			Engine::Log(LogError, "Could not load glfw!");
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
			Engine::Log(LogError, "Could not create glfw Window!");
			assert(false);
		}

		this->inputDevice = inputDevice;
		glfwSetKeyCallback(window, KeyCallbackForwarder);
		glfwSetCursorPosCallback(window, MousePosCallbackForwarder);
		glfwSetMouseButtonCallback(window, MouseButtonCallbackForwarder);
		glfwSetScrollCallback(window, MouseScrollCallbackForwarder);
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
		static bool isIlInit = false;
		if (!isIlInit)
		{
			isIlInit = true;
			ilInit();
			iluInit();
			ilutInit();
			ilutRenderer(ILUT_OPENGL);
		}

		printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
			   glGetString(GL_SHADING_LANGUAGE_VERSION));
		glClearColor(0.f, 0.f, 0.f, 0.f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
	}

	Window::~Window()
	{
		delete inputDevice;
		glfwTerminate();
	}

	void Window::Run()
	{
		while (!glfwWindowShouldClose(window))
		{
#if SHOW_STATS
			std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
#endif
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Update();
			Engine::Instance()->Update();

			glfwSwapBuffers(window);

			glfwPollEvents();

#if SHOW_STATS
			std::chrono::time_point<std::chrono::system_clock> foo = std::chrono::system_clock::now();

			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(foo - now);

			const int milliseconds = ms.count();
			Solus::Engine::Instance()->Log(LogLevel::LogDebug, "%d", milliseconds);
#endif
		}
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void Window::KeyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Engine::Instance()->GetInputDevice()->KeyEventUpdate(window, key, scancode, action, mods);
	}

	void Window::MousePosCallbackForwarder(GLFWwindow* window, double xPos, double yPos)
	{
		Engine::Instance()->GetInputDevice()->MousePosEventUpdate(window, xPos, yPos);
	}

	void Window::MouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods)
	{
		Engine::Instance()->GetInputDevice()->MouseButtonEventUpdate(window, button, action, mods);
	}

	void Window::MouseScrollCallbackForwarder(GLFWwindow* window, double xOffset, double yOffset)
	{
		Engine::Instance()->GetInputDevice()->MouseScrollEventUpdate(window, xOffset, yOffset);
	}
}
