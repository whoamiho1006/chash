#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int md5()
{
	if (auto MD5 = create(algorithm::MD5)) {
		digest_t Digest;

		if (MD5->init()) {
			MD5->update((uint8_t*)"abcd", 4);
			MD5->finalize(Digest);

			printf("MD5(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}