#include "chash/chash.hpp"
#include "chash/hmac_function.hpp"

using namespace chash;

int hmac_md5()
{
	if (auto hmac = create_hmac(algorithm::MD5)) {
		digest_t Digest;

		if (hmac->init((uint8_t*)"1234", 4)) {
			hmac->update((uint8_t*)"abcd", 4);
			hmac->finalize(Digest);

			printf("HMAC MD5(abcd, 1234): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}