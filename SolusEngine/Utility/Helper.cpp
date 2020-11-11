#include "Helper.h"

#include <chrono>
#include <ctime>

#include <chrono>
#include <random>
#include <algorithm>

namespace Solus
{
	int64_t GetCurrentTime()
	{
		auto time = std::chrono::system_clock::now();
		int64_t currentTime = std::chrono::system_clock::to_time_t(time);
		return currentTime;
	}

	float GetRandom(float min, float max, unsigned int seed)
	{
		return (float)GetRandom((double)min, (double)max, seed);
	}

	double GetRandom(double min, double max, unsigned int seed)
	{
		int64_t _seed = seed;
		static bool isGlobalSeedSet = false;
		static std::mt19937 gRngGen;
		std::mt19937& usedGen = gRngGen;
		if (seed == -1)
		{
			if (!isGlobalSeedSet)
			{
				seed = (unsigned int)time(0);
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

	int32_t GetRandom(int32_t min, int32_t max, unsigned int seed)
	{
		if (seed == -1)
			seed = (unsigned int)time(0);
		std::mt19937 randomBase(seed);
		std::uniform_int_distribution<> random(min, max);
		return random(randomBase);
	}

	void ToLower(std::string& string)
	{
		std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	}


	bool StringCompareCase(const std::string& a, const std::string& b)
	{
		if (a.size() != b.size())
			return false;
		return std::equal(a.begin(), a.end(), b.begin(),
						  [](char a, char b)
						  {
							  return tolower(a) == tolower(b);
						  });
	}

}