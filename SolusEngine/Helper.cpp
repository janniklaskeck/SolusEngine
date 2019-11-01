#include "Helper.h"

#include <chrono>
#include <ctime>
#include <time.h>

#include <chrono>
#include <random>

namespace Solus
{
	int64_t GetCurrentTime()
	{
		auto time = std::chrono::system_clock::now();
		int64_t currentTime = std::chrono::system_clock::to_time_t(time);
		return currentTime;
	}

	uint64_t GenerateUUID()
	{
		static uint64_t currentId = 1;
		return ++currentId;
	}

	float GetRandom(float min, float max, int64_t seed)
	{
		return (float)GetRandom((double)min, (double)max, seed);
	}

	double GetRandom(double min, double max, int64_t seed)
	{
		int64_t _seed = seed;
		static bool isGlobalSeedSet = false;
		static std::mt19937 gRngGen;
		std::mt19937& usedGen = gRngGen;
		if (seed == -1)
		{
			if (!isGlobalSeedSet)
			{
				seed = time(0);
				isGlobalSeedSet = true;
			}
		}
		else
		{
			usedGen = std::mt19937(seed);
		}
		std::uniform_real_distribution<> random(min, max);
		return random(usedGen);
	}

	int32_t GetRandom(int32_t min, int32_t max, int64_t seed)
	{
		if (seed == -1)
			seed = time(0);
		std::mt19937 randomBase(seed);
		std::uniform_int_distribution<> random(min, max);
		return random(randomBase);
	}


}