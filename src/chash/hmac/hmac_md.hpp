#pragma once
#include "chash/hmac/hmac_impl.hpp"
#include "chash/md/md5.hpp"
#include "chash/md/md4.hpp"

namespace chash {
	/* hmac md5. */
	typedef hmac_impl<md5, 64> hmac_md5;

	/* hmac md4. */
	typedef hmac_impl<md4, 64> hmac_md4;
}