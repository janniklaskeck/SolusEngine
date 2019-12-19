#pragma once
#include "Engine/SolusEngine.h"

namespace Solus
{

	class SOLUS_API Timer
	{
	public:
		Timer();

		void Update();

		bool TickDone() const;
		void Reset();

		double GetDeltaTime() const;
	private:
		//static double fixedTimeStep;
		bool tickDone = false;
		double lastTickTimeLog;
		double lastTickTime;
		double currentTickTime;
		int frameCount;
	};
}

