#pragma once

#include "Engine/SolusEngine.h"

#include "Vector.h"

#include <cstdint>
#include <string>

namespace Solus
{
	int64_t GetCurrentTime();

	float GetRandom(float min, float max, unsigned int seed = -1);

	double GetRandom(double min, double max, unsigned int seed = -1);

	int32_t GetRandom(int32_t min, int32_t max, unsigned int seed = -1);

	void ToLower(std::string& string);

	bool StringCompareCase(const std::string& a, const std::string& b);

	void ReplaceChar(std::string& string, const char toReplace, const char replacement);

	class SOLUS_API Rect
	{
	public:
		Rect(Vec2i center, Vec2i extents)
			: center(center), extents(extents)
		{}

		Vec2i TopLeft() const
		{
			return Vec2i(center.x - extents.x, center.y + extents.y);
		}

		Vec2i TopRight() const
		{
			return Vec2i(center.x + extents.x, center.y + extents.y);
		}

		Vec2i BottomLeft() const
		{
			return Vec2i(center.x - extents.x, center.y - extents.y);
		}

		Vec2i BottomRight() const
		{
			return Vec2i(center.x + extents.x, center.y - extents.y);
		}

		int32_t Width() const
		{
			return extents.x * 2;
		}

		int32_t Height() const
		{
			return extents.y * 2;
		}
	private:
		Vec2i center;
		Vec2i extents;
	};
}

