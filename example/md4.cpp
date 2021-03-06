#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int md4()
{
	if (auto MD4 = create(algorithm::MD4)) {
		digest_t Digest;

		if (MD4->init()) {
			MD4->update((uint8_t*)"abcd", 4);
			MD4->finalize(Digest);

			printf("MD5(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}