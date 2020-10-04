#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int sha256()
{

	if (IAlgorithm* SHA256 = createAlgorithm(EAlgorithm::SHA256)) {
		CDigest Digest;

		if (SHA256->init()) {
			SHA256->update((uint8_t*)"abcd", 4);
			SHA256->finalize(Digest);

			printf("SHA256(abcd): %s\n", toHex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}