#pragma once

#include "SolusEngine.h"
#include "ProjectFile.h"

#include "Object/Entity.h"

#include "Render/Window.h"
#include "Render/RenderDevice.h"

#include "Input/InputDevice.h"

#include <cstdint>
#include <vector>
#include <memory>

namespace Solus
{

	enum class LogLevel : uint8_t
	{
		LogVerbose = 0,
		LogDebug,
		LogWarning,
		LogError
	};

	class Window;
	class Camera;
	class Timer;
	class World;
	class LogListener;
	class AssetManager;

	void SOLUS_API InitializeEngine(Window* window, const std::string& projectFilePath);

	class SOLUS_API Engine : public SubSystem
	{
	public:
		Engine();
		~Engine();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		void Log(LogLevel level, const char* msgFormat, ...);
		void AddLogListener(LogListener* listener);
		void RemoveLogListener(LogListener* listener);

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

		const Timer* GetTickTimer() const;
		const Timer* GetRenderTimer() const;

		double DeltaTime() const;

		void SetCurrentProject(const std::string& projectFile);
		const ProjectFile* GetCurrentProject() const;
	private:

		std::unique_ptr<ProjectFile> currentProject;

		static const char* LogLevelToChar(LogLevel level);

		std::unique_ptr<Window> window;
		std::unique_ptr<RenderDevice> renderDevice;

		InputDevice* inputDevice = nullptr;
		std::unique_ptr<World> world;
		Camera* mainCamera = nullptr;

		std::unique_ptr<AssetManager> assetManager;

		std::unique_ptr<Timer> tickTimer;
		std::unique_ptr<Timer> renderTimer;

	private:
		int tickCounter = 0;
		std::vector<LogListener*> logListeners;
	};

	extern SOLUS_API Engine* gEngine;
}
