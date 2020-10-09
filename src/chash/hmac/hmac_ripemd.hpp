#pragma once
#include "chash/hmac/hmac_impl.hpp"
#include "chash/ripemd/ripemd128.hpp"
#include "chash/ripemd/ripemd160.hpp"

namespace chash {
	/* hmac ripemd128. */
	typedef hmac_impl<ripemd128, 64> hmac_ripemd128;

	/* hmac ripemd160. */
	typedef hmac_impl<ripemd160, 64> hmac_ripemd160;
}