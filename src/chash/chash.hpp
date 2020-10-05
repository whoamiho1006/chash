#pragma once
#include "chash/boilerplate.hpp"
#include "chash/hash_function.hpp"
#include <memory>

namespace chash {
	CHASH_API std::unique_ptr<hash_function> create(algorithm algorithm);
}