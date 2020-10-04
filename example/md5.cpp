#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int md5()
{
	if (IAlgorithm* MD5 = createAlgorithm(EAlgorithm::MD5)) {
		CDigest Digest;

		if (MD5->init()) {
			MD5->update((uint8_t*)"abcd", 4);
			MD5->finalize(Digest);

			printf("MD5(abcd): %s\n", toHex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}