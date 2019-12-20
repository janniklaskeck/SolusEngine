#pragma once
#include "Engine/SolusEngine.h"

namespace Solus
{

	class SOLUS_API Timer : public SubSystem
	{
	public:
		Timer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

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

