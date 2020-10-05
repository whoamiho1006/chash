#pragma once
#include "chash/Macros.hpp"
#include "chash/IAlgorithm.hpp"
#include <memory>

namespace chash {
	CHASH_API std::unique_ptr<hash_function> create(algorithm algorithm);
}