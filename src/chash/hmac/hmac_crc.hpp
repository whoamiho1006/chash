#pragma once
#include "chash/hmac/hmac_impl.hpp"
#include "chash/crc/crc16.hpp"
#include "chash/crc/crc32.hpp"
#include "chash/crc/crc64.hpp"

namespace chash {
	/* hmac crc16. */
	typedef hmac_impl<crc16, 64> hmac_crc16;

	/* hmac crc32. */
	typedef hmac_impl<crc32, 64> hmac_crc32;

	/* hmac crc64. */
	typedef hmac_impl<crc64, 64> hmac_crc64;
}