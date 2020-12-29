#pragma once
#include "Engine/SolusEngine.h"

namespace Solus
{

	class SOLUS_API Timer : public SubSystem
	{
	public:
		Timer();
		~Timer()
		{}

		void Initialize() override;
		void Update() override;
		void Destroy() override;

		bool TickDone() const;
		void Reset();

		double GetDeltaTime() const;
	private:
		static double fixedTimeStep;
		bool tickDone = false;
		double lastTickTime;
		double currentTickTime;
	};
}

