#pragma once

#include <cstdint>

namespace Solus
{
	enum class SAssetType : uint8_t
	{
		NONE = 0,
		TEXTURE,
		MESH,
		SHADER,
		MATERIAL,
		SOUND,
		LEVEL,
		OBJECT
	};
}
