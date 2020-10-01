#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

#if defined(_WIN32) || defined(_WIN64)
#ifdef __CHASH_EXPORTS__
#define CHASH_API __declspec(dllexport)
#else
#define CHASH_API __declspec(dllimport)
#endif
#else
#define CHASH_API
#endif

namespace chash {
	CHASH_API IAlgorithm* createAlgorithm(EAlgorithm algorithm);
}