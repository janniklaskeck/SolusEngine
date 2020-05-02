#include "Timer.h"
#include "Engine/Engine.h"

namespace Solus
{
	double Timer::fixedTimeStep = 1.0 / 60.0;

	Timer::Timer()
		: currentTickTime(0),
		lastTickTime(0)
	{}

	void Timer::Initialize()
	{
		lastTickTime = gEngine->GetWindow()->GetTime();
	}

	void Timer::Update()
	{
		auto currentTime = gEngine->GetWindow()->GetTime();
		auto delta = currentTime - lastTickTime;
		tickDone = delta >= fixedTimeStep;
		if (tickDone)
		{
			lastTickTime = currentTime;
			currentTickTime = delta;
		}
	}

	void Timer::Destroy()
	{}

	bool Timer::TickDone() const
	{
		return tickDone;
	}

	void Timer::Reset()
	{
		lastTickTime = gEngine->GetWindow()->GetTime();
	}

	double Timer::GetDeltaTime() const
	{
		return currentTickTime;
	}
}
