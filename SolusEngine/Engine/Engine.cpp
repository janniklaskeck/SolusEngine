#include "Engine.h"

#include "Render/GLFWWindow.h"
#include "Render/Window.h"
#include "Render/OpenGLDevice.h"
#include "Render/Camera.h"

#include "Input/InputDevice.h"

#include "Object/World.h"

#include "AssetSystem/FolderAssetSource.h"

#include "Utility/Timer.h"
#include "Utility/FileUtils.h"

#include "LogListener.h"

#include <iostream>
#include <cstdarg>
#include <ctime>
#include <ostream>
#include <chrono>
#include <ctime>

namespace Solus
{
	Engine* gEngine = nullptr;

	void InitializeEngine(Window* window)
	{
		gEngine = new Engine;
		gEngine->InitWindow(window); // Do Window first, necessary for now
		gEngine->Initialize();
	}

	void Engine::Initialize()
	{
		// Add Engine assets
		assetManager = new AssetManager;
		assetManager->Initialize();
		auto engineAssetRoot = FileUtils::GetCurrentFolder() + "/../Assets";
		gEngine->GetAssetManager()->SetEngineAssetRoot(engineAssetRoot);

		world = new World;
		window->Initialize();

		tickTimer = new Timer;
		tickTimer->Initialize();

		renderTimer = new Timer;
		renderTimer->Initialize();

		renderDevice = new OpenGLDevice;
		renderDevice->Initialize();


		auto now = std::chrono::system_clock::now();
		std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
		char buffer[64];
		ctime_s(buffer, 64, &nowTime);
		gEngine->Log(LogLevel::LogDebug, "Engine initialized at: %s", buffer);
	}

	Engine::Engine()
	{}

	Engine::~Engine()
	{
		delete tickTimer;
		delete renderTimer;
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
		for (auto* listener : logListeners)
		{
			listener->OnLogReceived(level, buf);
		}
		va_end(args);
	}

	void Engine::AddLogListener(LogListener* listener)
	{
		// TODO checks
		logListeners.push_back(listener);
	}

	void Engine::RemoveLogListener(LogListener* listener)
	{
		// TODO checks
		logListeners.erase(std::find(logListeners.begin(), logListeners.end(), listener));
	}

	void Engine::Update()
	{
		tickTimer->Update();
		if (tickTimer->TickDone())
		{
			GetInputDevice()->Update();
			GetWorld()->Update((float)tickTimer->GetDeltaTime());
			GetWindow()->Update();
		}
	}

	void Engine::Render()
	{
		renderTimer->Update();
		//if (renderTimer->TickDone())
		{
			auto* renderDevice = GetRenderDevice();
			renderDevice->PreRenderScene();
			GetWorld()->Render();
			renderDevice->PostRenderScene();

			GetWindow()->Render();
		}
	}

	void Engine::Destroy()
	{
		tickTimer->Destroy();
		assetManager->Destroy();
		renderDevice->Destroy();
		inputDevice->Destroy();
		window->Destroy();
	}

	const char* Engine::LogLevelToChar(LogLevel level)
	{
		switch (level)
		{
		case Solus::LogLevel::LogVerbose:
			return "V";
		case Solus::LogLevel::LogDebug:
			return "D";
		case Solus::LogLevel::LogWarning:
			return "W";
		case Solus::LogLevel::LogError:
			return "E";
		default:
			return "Unknown";
		}
	}

	void Engine::InitWindow(Window* windowInstance)
	{
		window = windowInstance;
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

	const Timer* Engine::GetTickTimer() const
	{
		return tickTimer;
	}

	const Timer* Engine::GetRenderTimer() const
	{
		return renderTimer;
	}

	double Engine::DeltaTime() const
	{
		return tickTimer->GetDeltaTime();
	}

}
