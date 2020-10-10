#include "chash/chash.hpp"
#include "chash/hmac_function.hpp"

using namespace chash;

int hmac_sha512()
{
	if (auto hmac = create_hmac(algorithm::SHA512)) {
		digest_t Digest;

		if (hmac->init((uint8_t*)"1234", 4)) {
			hmac->update((uint8_t*)"abcd", 4);
			hmac->finalize(Digest);

			printf("HMAC SHA512(abcd, 1234): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}