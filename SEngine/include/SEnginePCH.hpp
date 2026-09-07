#pragma once

#ifdef SOLUSENGINE_EXPORT
//#define SOLUS_API __declspec(dllexport)
#else
//#define SOLUS_API __declspec(dllimport)
#endif

#define SOLUS_API

//////////////////////////////////////////////////////////////////////////

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//////////////////////////////////////////////////////////////////////////

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <SDL3/SDL.h>

//////////////////////////////////////////////////////////////////////////

#include "Utils/STypes.hpp"

#include "core/Ref.hpp"
#include "core/SEngine.hpp"

#include "core/math/SMath.hpp"

#include "entity/SEntity.hpp"

#include "entity/component/SEntityComponent.hpp"
#include "entity/component/STagComponent.hpp"
#include "entity/component/STransformComponent.hpp"

#include "assets/SAsset.hpp"
#include "assets/SMeshAsset.hpp"
#include "assets/SShaderProgramAsset.hpp"

#include "render/camera/SCamera.hpp"


//////////////////////////////////////////////////////////////////////////

