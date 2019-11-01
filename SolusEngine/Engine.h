#pragma once

#include "SolusEngine.h"

#include "Entity.h"
#include "Vector.h"

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

	class SOLUS_API Engine
	{
		Engine();
	public:
		~Engine();

		static Engine* Instance();
		static bool Initialize(class Window* mainWindow);

		static void Log(LogLevel level, const char* msgFormat, ...);

		void Update();

		void InitMainWindow(Window* windowInstance);
		Window* GetMainWindow() const;

		void SetWorld(class World* newWorld);
		class World* GetWorld() const;

		class RenderDevice* GetRenderDevice();

		class Camera* GetMainCamera() const;
		void SetMainCamera(Camera* camera);

		class InputDevice* GetInputDevice() const;
	private:
		static Engine* engineInstance;
		static const char* LogLevelToChar(LogLevel level);

		class Window* mainWindow = nullptr;
		class World* world = nullptr;
		class RenderDevice* renderDevice = nullptr;
		class Camera* mainCamera = nullptr;
	};
}
