#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int sha256()
{

	if (auto SHA256 = create(algorithm::SHA256)) {
		digest_t Digest;

		if (SHA256->init()) {
			SHA256->update((uint8_t*)"abcd", 4);
			SHA256->finalize(Digest);

			printf("SHA256(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}