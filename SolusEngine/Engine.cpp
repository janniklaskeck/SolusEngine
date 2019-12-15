#include "Engine.h"

#include "GLFWWindow.h"
#include "Window.h"
#include "OpenGLDevice.h"

#include "InputDevice.h"

#include "World.h"
#include "Camera.h"

#include "Helper.h"
#include "Timer.h"

#include "FileUtils.h"

#include <iostream>
#include <cstdarg>
#include <ctime>
#include <ostream>
#include <chrono>
#include <ctime>
#include "FolderAssetSource.h"

namespace Solus
{
	Engine* gEngine = nullptr;

	bool Engine::Initialize()
	{
		gEngine = new Engine;
		gEngine->mainTimer = new Timer;

		auto now = std::chrono::system_clock::now();
		std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
		char buffer[64];
		ctime_s(buffer, 64, &nowTime);
		gEngine->Log(LogDebug, "Engine initialized at: %s", buffer);

		return true;
	}

	Engine::Engine()
	{
		gEngine = this;
		assetManager = new AssetManager;
		auto a = FileUtils::GetCurrentFolder();
		FolderAssetSource* baseAssetFolder = new FolderAssetSource(a + "/../editor");
		gEngine->GetAssetManager()->AddSource(baseAssetFolder);
		RegisterRenderWindow(new GLFWWindow);
		world = new World;
		renderDevice = new OpenGLDevice;
		renderDevice->Initialize();
	}

	Engine::~Engine()
	{
		delete renderWindow;
		delete window;
		delete world;
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
		mainTimer->Update();
		//if (mainTimer->TickDone())
		//{
			//mainTimer->Reset();
			GetInputDevice()->Update();
			GetWorld()->Update();
			GetWindow()->Update();
			GetRenderWindow()->Update();

		//}
	}

	void Engine::Render()
	{
		renderDevice->PreRenderScene();
		GetWorld()->Render();
		renderDevice->PostRenderScene();
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

	void Engine::InitRenderWindow(RenderWindow* windowInstance)
	{
		renderWindow = windowInstance;
	}

	void Engine::RegisterRenderWindow(RenderWindow* newRenderWindow)
	{
		renderWindow = newRenderWindow;
		renderWindow->Initialize();
	}

	RenderWindow* Engine::GetRenderWindow() const
	{
		return renderWindow;
	}

	void Engine::InitWindow(Window* windowInstance)
	{
		window = windowInstance;
		window->Initialize();
	}

	Window* Engine::GetWindow() const
	{
		return window;
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

	void Engine::RegisterInputDevice(InputDevice* newInputDevice)
	{
		inputDevice = newInputDevice;
	}

	InputDevice* Engine::GetInputDevice() const
	{
		return inputDevice;
	}

	AssetManager* Engine::GetAssetManager() const
	{
		return assetManager;
	}

	Timer* Engine::GetMainTimer() const
	{
		return mainTimer;
	}

	double Engine::DeltaTime() const
	{
		return mainTimer->GetDeltaTime();
	}

}
