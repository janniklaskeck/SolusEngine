#pragma once

#include "SolusEngine.h"
#include "AssetManager.h"

#include "Entity.h"
#include "Vector.h"

#include "Window.h"
#include "RenderWindow.h"

#include <cstdint>

namespace Solus
{

	enum LogLevel : uint8_t
	{
		LogVerbose = 0,
		LogDebug,
		LogWarning,
		LogError
	};

	class World;
	class InputDevice;
	class Camera;
	class RenderDevice;
	class AssetManager;
	class Timer;

	class SOLUS_API Engine
	{
		Engine();
	public:
		~Engine();

		static bool Initialize();

		void Log(LogLevel level, const char* msgFormat, ...);

		void Update();

		void Render();

	private:
		void InitRenderWindow(RenderWindow* renderWindowInstance);
	public:
		void RegisterRenderWindow(RenderWindow* newRenderWindow);
		RenderWindow* GetRenderWindow() const;

		void InitWindow(Window* windowInstance);
		Window* GetWindow() const;

		void SetWorld(World* newWorld);
		World* GetWorld() const;

		RenderDevice* GetRenderDevice();

		Camera* GetMainCamera() const;
		void SetMainCamera(Camera* camera);

		void RegisterInputDevice(InputDevice* newInputDevice);
		InputDevice* GetInputDevice() const;

		AssetManager* GetAssetManager() const;

		Timer* GetMainTimer() const;

		double DeltaTime() const;
	private:
		static Engine* engineInstance;
		static const char* LogLevelToChar(LogLevel level);

		RenderWindow* renderWindow = nullptr;
		Window* window = nullptr;
		RenderDevice* renderDevice = nullptr;
		
		InputDevice* inputDevice = nullptr;
		World* world = nullptr;
		Camera* mainCamera = nullptr;

		AssetManager* assetManager = nullptr;

		Timer* mainTimer = nullptr;
	private:
		int tickCounter = 0;
	};

	extern SOLUS_API Engine* gEngine;
}
