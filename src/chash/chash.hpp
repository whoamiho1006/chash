#pragma once
#include "chash/Macros.hpp"
#include "chash/IAlgorithm.hpp"
#include <memory>

namespace chash {
	CHASH_API std::unique_ptr<IAlgorithm> createAlgorithm(EAlgorithm algorithm);
}