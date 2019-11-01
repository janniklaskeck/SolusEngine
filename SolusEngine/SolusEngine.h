#pragma once

#define NOMINMAX

#ifdef SOLUSENGINE_EXPORT
#define SOLUS_API __declspec(dllexport)
#else
#define SOLUS_API __declspec(dllimport)
#endif
