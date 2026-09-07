#pragma once

#include "SCameraData.hpp"

namespace Solus
{

	class SCamera
	{
	public:

		SCamera() = default;

	public:

		STransform Transform{};

		SCameraData Data;
	};

}