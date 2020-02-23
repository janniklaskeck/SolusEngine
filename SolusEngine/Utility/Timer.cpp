#include "Timer.h"
#include "Engine/Engine.h"

namespace Solus
{
	//double Timer::fixedTimeStep = 1.0 / 60.0;

	Timer::Timer()
		: currentTickTime(0),
		lastTickTime(0)
	{}

	void Timer::Initialize()
	{
		currentTickTime = gEngine->GetWindow()->GetTime();
		lastTickTime = currentTickTime;
	}

	void Timer::Update()
	{
		lastTickTime = currentTickTime;
		currentTickTime = gEngine->GetWindow()->GetTime();

		//tickDone = (diff) >= fixedTimeStep;
	}

	void Timer::Destroy()
	{}

	bool Timer::TickDone() const
	{
		return tickDone;
	}

	void Timer::Reset()
	{
		lastTickTime = currentTickTime;
	}
	double Timer::GetDeltaTime() const
	{
		return currentTickTime - lastTickTime;
	}
}
