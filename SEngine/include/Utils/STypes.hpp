#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include <SDL3/SDL.h>

#include <filesystem>
#include <memory>

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using SString = std::string;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using IVec2 = glm::ivec2;
using IVec3 = glm::ivec3;
using IVec4 = glm::ivec4;

using UVec2 = glm::uvec2;
using UVec3 = glm::uvec3;
using UVec4 = glm::uvec4;

using Quaternion = glm::quat;

using SFColor = SDL_FColor;
using SColor = SDL_Color;

using SAssetPath = std::filesystem::path;
using SAssetID = uint32;

namespace Vec
{
	constexpr Vec3 ForwardVector = Vec3{ 0.f, 0.f, 1.f };
	constexpr Vec3 RightVector = Vec3{ 1.f, 0.f, 0.f };
	constexpr Vec3 UpVector = Vec3{ 0.f, 1.f, 0.f };
}

#define SLOG(fmt, ...) SDL_Log(fmt, ##__VA_ARGS__)

namespace Solus
{
	namespace String
	{
		SString Printf(const char* Format, ...);
	}

	namespace Color
	{
		constexpr SFColor White = SFColor{ 1.f, 1.f, 1.f, 1.f};
		constexpr SFColor Black = SFColor{ 0.f, 0.f, 0.f, 1.f };
	}

	struct SVertex
	{
		Vec3 Position{};
		SColor Color{};
	};

}
