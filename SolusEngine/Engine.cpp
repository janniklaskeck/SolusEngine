#include "Engine.h"

#include "Helper.h"
#include "Window.h"
#include "InputDevice.h"
#include "World.h"
#include "OpenGLDevice.h"
#include "Camera.h"

#include <iostream>
#include <cstdarg>
#include <ctime>
#include <ostream>

namespace Solus
{

	Engine* Engine::engineInstance = nullptr;

	Engine::Engine()
	{}


	Engine::~Engine()
	{
		delete mainWindow;
		delete world;
	}

	Engine* Engine::Instance()
	{
		if (!engineInstance)
			engineInstance = new Engine;
		return engineInstance;
	}

	bool Engine::Initialize(Window* mainWindow)
	{
		int64_t currentTime = GetCurrentTime();
		char buf[26];
		ctime_s(buf, 26, &currentTime);
		Log(LogDebug, "Engine initialized at: %s", buf);

		Instance()->mainWindow = mainWindow;
		Instance()->world = new World;

		return false;
	}

	void Engine::Log(LogLevel level, const char* msgFormat, ...)
	{
		va_list args;
		va_start(args, msgFormat);
		char buf[1024];
		vsprintf_s(buf, msgFormat, args);
		printf("[%s] %s \n", LogLevelToChar(level), buf);
		va_end(args);
	}

	void Engine::Update()
	{
		GetInputDevice()->Update();
		GetWorld()->Update();
		GetWorld()->Render();
	}

	const char* Engine::LogLevelToChar(LogLevel level)
	{
		switch (level)
		{
		case Solus::LogVerbose:
			return "V";
		case Solus::LogDebug:
			return "D";
		case Solus::LogWarning:
			return "W";
		case Solus::LogError:
			return "E";
		default:
			return "Unknown";
		}
	}

	void Engine::InitMainWindow(Window* windowInstance)
	{
		mainWindow = windowInstance;
	}

	Window* Engine::GetMainWindow() const
	{
		return mainWindow;
	}

	void Engine::SetWorld(World* newWorld)
	{
		world = newWorld;
	}

	World* Engine::GetWorld() const
	{
		return world;
	}

	RenderDevice* Engine::GetRenderDevice()
	{
		if (!renderDevice)
			renderDevice = new OpenGLDevice;
		return renderDevice;
	}

	Camera* Engine::GetMainCamera() const
	{
		return mainCamera;
	}

	void Engine::SetMainCamera(Camera* camera)
	{
		mainCamera = camera;
	}

	InputDevice* Engine::GetInputDevice() const
	{
		return mainWindow->GetInputDevice();
	}

}
