#pragma once

#include "bx/timer.h"
#include "processor/SProcessorManager.hpp"

namespace Solus
{
	class SInputProcessor;
	class SProcessorManager;
	class SWorld;
	class SWindow;
	class SRenderingProcessor;
	class SPhysicsWorld;
	class SCameraProcessor;

	class SEngine final
	{
	public:
		SOLUS_API SEngine();

		SOLUS_API ~SEngine();

		SOLUS_API void Initialize();

		SOLUS_API bool Tick();

		SOLUS_API void Shutdown();

	public:

		SWindow* GetWindow() const;

		SWorld& GetWorld() const;
		
		SPhysicsWorld& GetPhysicsWorld() const;

		SProcessorManager& GetProcessorManager() const;

		template<typename T>
		T& GetProcessor() const
		{
			return ProcessorManager->Get<T>();
		}

	public:

		void ShowDebugMessage(const SString& Msg, const float Duration);

	public:

		int32 GetFPS() const;

		int32 GetTargetFPS() const;

		float GetDeltaTime() const;

	private:

		std::unique_ptr<SWindow> Window;

		std::unique_ptr<SWorld> ActiveWorld;
		std::unique_ptr<SPhysicsWorld> PhysicsWorld;

		std::unique_ptr<SProcessorManager> ProcessorManager;

		bx::Ticks TicksStart{ bx::InitNone };
		bx::Ticks TicksPrevious{ bx::InitNone };
		bx::Ticks TicksCurrent{ bx::InitNone };
	};

	extern SOLUS_API SEngine* gEngine;
}
