#include "Timer.h"
#include "Engine/Engine.h"

namespace Solus
{
	//double Timer::fixedTimeStep = 1.0 / 60.0;

	Timer::Timer()
		: currentTickTime(0),
		lastTickTime(0),
		lastTickTimeLog(0),
		frameCount(0)
	{}

	void Timer::Initialize()
	{
		currentTickTime = gEngine->GetWindow()->GetTime();
		lastTickTime = currentTickTime;
		lastTickTimeLog = lastTickTime;
	}

	void Timer::Update()
	{
		lastTickTime = currentTickTime;
		currentTickTime = gEngine->GetWindow()->GetTime();
		auto diff = currentTickTime - lastTickTime;

		frameCount++;
		if (currentTickTime - lastTickTimeLog > 1.0)
		{
			float msPerFrame = 1000.0f / float(frameCount);
			gEngine->Log(LogVerbose, "%f ms/frame (%d FPS)", msPerFrame, int(1000.f / msPerFrame));
			frameCount = 0;
			lastTickTimeLog += 1.0;
		}
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
