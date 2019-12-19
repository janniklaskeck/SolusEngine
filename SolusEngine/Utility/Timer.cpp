#include "Timer.h"
#include "Engine/Engine.h"

namespace Solus
{
	//double Timer::fixedTimeStep = 1.0 / 60.0;

	Timer::Timer()
	{
		currentTickTime = gEngine->GetRenderWindow()->GetTime();
		lastTickTime = currentTickTime;
		lastTickTimeLog = lastTickTime;
	}

	void Timer::Update()
	{
		lastTickTime = currentTickTime;
		currentTickTime = gEngine->GetRenderWindow()->GetTime();
		auto diff = currentTickTime - lastTickTime;

		frameCount++;
		if (currentTickTime - lastTickTimeLog > 1.0)
		{
			float msPerFrame = 1000.0f / float(frameCount);
			gEngine->Log(LogVerbose, "%f ms/frame (%d FPS)", msPerFrame, int(1000.f/ msPerFrame));
			frameCount = 0;
			lastTickTimeLog += 1.0;
		}
		//tickDone = (diff) >= fixedTimeStep;
	}

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
