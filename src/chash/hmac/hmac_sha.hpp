#pragma once
#include "chash/hmac/hmac_impl.hpp"
#include "chash/sha/sha256.hpp"
#include "chash/sha/sha384.hpp"
#include "chash/sha/sha512.hpp"

namespace chash {
	/* hmac sha256. */
	typedef hmac_impl<sha256, 64> hmac_sha256;

	/* hmac sha384. */
	typedef hmac_impl<sha384, 128> hmac_sha384;

	/* hmac sha512. */
	typedef hmac_impl<sha512, 128> hmac_sha512;
}