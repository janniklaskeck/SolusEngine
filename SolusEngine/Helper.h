#pragma once

#include "SolusEngine.h"

#include <cstdint>
#include <string>

namespace Solus
{
	int64_t GetCurrentTime();

	uint64_t GenerateUUID();

	float GetRandom(float min, float max, int64_t seed = -1);

	double GetRandom(double min, double max, int64_t seed = -1);

	int32_t GetRandom(int32_t min, int32_t max, int64_t seed = -1);

	void ToLower(std::string& string);
}

