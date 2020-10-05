#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int ripemd160()
{
	if (auto RipeMD160 = create(algorithm::RipeMD160)) {
		digest_t Digest;

		if (RipeMD160->init()) {
			RipeMD160->update((uint8_t*)"abcd", 4);
			RipeMD160->finalize(Digest);

			printf("RipeMD160(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}