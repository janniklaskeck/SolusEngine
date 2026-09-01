#include "core/SEngine.hpp"

#include "assets/SAssetManager.hpp"
#include "core/physics/SPhysicsWorld.hpp"
#include "core/processor/SCameraProcessor.hpp"
#include "core/processor/SInputProcessor.hpp"
#include "core/processor/SPhysicsProcessor.hpp"
#include "core/processor/SProcessorManager.hpp"
#include "core/SWorld.hpp"
#include "entity/component/SCameraComponent.hpp"
#include "entity/component/SMeshComponent.hpp"
#include "entity/component/SRigidBodyComponent.hpp"
#include "entity/component/STransformComponent.hpp"
#include "entity/SEntity.hpp"
#include "core/processor/SRenderingProcessor.hpp"
#include "render/SWindow.hpp"

#include <bx/os.h>
#include <bx/string.h>
#include <SDL3/SDL.h>

namespace Solus
{
	SEngine* gEngine = nullptr;

	SEngine::SEngine()
	{
	}

	SEngine::~SEngine()
	{

	}

	void SEngine::Initialize()
	{
		gEngine = this;

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);		

		SAssetPath AssetRootPath = std::filesystem::current_path() / "assets";
		SLOG("%s", AssetRootPath.string().c_str());
		SAssetManager::Get().Init(AssetRootPath, 1);

		Window = std::make_unique<SWindow>("Test", 1280, 720);

		ActiveWorld = std::make_unique<SWorld>();
		PhysicsWorld = std::make_unique<SPhysicsWorld>();

		ProcessorManager = std::make_unique<SProcessorManager>();
		ProcessorManager->Emplace<SCameraProcessor>();
		ProcessorManager->Emplace<SPhysicsProcessor>();
		ProcessorManager->Emplace<SInputProcessor>();
		ProcessorManager->Emplace<SRenderingProcessor>();

		SEntity CameraEntity = ActiveWorld->CreateEntity({ 0.0f, 0.0f, -35.0f });
		CameraEntity.AddComponent<SCameraComponent>();

		TicksCurrent = bx::getNow();
		TicksStart = TicksCurrent;
	}

	bool SEngine::Tick()
	{
		TicksPrevious = TicksCurrent;
		TicksCurrent = bx::getNow();

		const float DeltaTime = GetDeltaTime();

		SInputProcessor& InputProcessor = ProcessorManager->Get<SInputProcessor>();

		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			InputProcessor.QueueEvent(Event);

			if (Event.type == SDL_EVENT_QUIT)
			{
				return false;
			}
		}

		if (InputProcessor.IsKeyDown(SDLK_ESCAPE))
		{
			return false;
		}

		ShowDebugMessage(String::Printf("FPS: %d (Delta: %.6f)", GetFPS(), DeltaTime), 0.f);

		ProcessorManager->Tick(DeltaTime);

		const bx::Ticks WorkTicks = bx::getNow() - TicksCurrent;
		const double WorkTime = bx::toSeconds<double>(WorkTicks);
		
		const double TargetFrameTime = 1.0 / (double)GetTargetFPS();
		if (WorkTime < TargetFrameTime)
		{
			const double TimeDelay = (TargetFrameTime - WorkTime) * 1000.0;
			bx::sleep(TimeDelay);
		}

		return true;
	}

	void SEngine::Shutdown()
	{
		ProcessorManager.reset();

		ActiveWorld.reset();

		Window.reset();

		SDL_Quit();
	}

	SWindow* SEngine::GetWindow() const
	{
		return Window.get();
	}

	SWorld& SEngine::GetWorld() const
	{
		return *ActiveWorld;
	}

	SPhysicsWorld& SEngine::GetPhysicsWorld() const
	{
		return *PhysicsWorld;
	}

	SProcessorManager& SEngine::GetProcessorManager() const
	{
		return *ProcessorManager;
	}

	void SEngine::ShowDebugMessage(const SString& Msg, const float Duration)
	{
		ProcessorManager->Get<SRenderingProcessor>().RenderDebugMessage(Msg, Duration);
	}

	int32 SEngine::GetFPS() const
	{
		return 1.f / GetDeltaTime();
	}

	int32 SEngine::GetTargetFPS() const
	{
		return 60;
	}

	float SEngine::GetDeltaTime() const
	{
		return bx::toSeconds<float>(TicksCurrent - TicksPrevious);
	}

}
