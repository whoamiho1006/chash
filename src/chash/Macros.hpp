#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <exception>

#if defined(_WIN32) || defined(_WIN64)
#ifdef __CHASH_EXPORTS__
#define CHASH_API __declspec(dllexport)
#else
#define CHASH_API __declspec(dllimport)
#endif
#else
#define CHASH_API
#endif

#pragma warning (disable: 4275)