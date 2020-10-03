#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int md4()
{
	if (IAlgorithm* MD4 = createAlgorithm(EAlgorithm::MD4)) {
		IDigest* Digest = MD4->create();

		if (MD4->init()) {
			MD4->update((uint8_t*)"abcd", 4);
			MD4->finalize(Digest);

			printf("MD4(abcd): %s\n", Digest->toHex().c_str());
			return 0;
		}
	}

	return -1;
}