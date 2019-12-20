#pragma once

#include "SolusEngine.h"

#include "AssetSystem/AssetManager.h"

#include "Object/Entity.h"
#include "Object/World.h"

#include "Render/Window.h"
#include "Render/RenderDevice.h"

#include "Input/InputDevice.h"

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

	class Window;
	class Camera;
	class Timer;

	void SOLUS_API InitializeEngine(Window* window);

	class SOLUS_API Engine : public SubSystem
	{
	public:
		Engine();
		~Engine();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		void Log(LogLevel level, const char* msgFormat, ...);


		void Render();

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
