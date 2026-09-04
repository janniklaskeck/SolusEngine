#pragma once

#ifdef SOLUSENGINE_EXPORT
//#define SOLUS_API __declspec(dllexport)
#else
//#define SOLUS_API __declspec(dllimport)
#endif

#define SOLUS_API

//////////////////////////////////////////////////////////////////////////

#include "Utils/STypes.hpp"

#include "core/Ref.hpp"
#include "core/SEngine.hpp"

#include "core/math/SMath.hpp"

#include "entity/SEntity.hpp"

#include "assets/SAsset.hpp"
#include "assets/SMeshAsset.hpp"
#include "assets/SShaderProgramAsset.hpp"

//////////////////////////////////////////////////////////////////////////

#include <memory>
#include <cstdint>
#include <string>

//////////////////////////////////////////////////////////////////////////

