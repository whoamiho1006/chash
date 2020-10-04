#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int ripemd160()
{
	if (IAlgorithm* RipeMD160 = createAlgorithm(EAlgorithm::RipeMD160)) {
		CDigest Digest;

		if (RipeMD160->init()) {
			RipeMD160->update((uint8_t*)"abcd", 4);
			RipeMD160->finalize(Digest);

			printf("RipeMD160(abcd): %s\n", toHex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}