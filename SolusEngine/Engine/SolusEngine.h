#pragma once

#define NOMINMAX

#ifdef SOLUSENGINE_EXPORT
#define SOLUS_API __declspec(dllexport)
#else
#define SOLUS_API __declspec(dllimport)
#endif

// GLOBAL INCLUDES

#include "Engine/SubSystem.h"
#include "Utility/UUID.h"

#include <cstdint>
#include <string>
#include <cassert>
#include <filesystem>
#include <vector>
#include <memory>

namespace fs = std::filesystem;