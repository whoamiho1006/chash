
#include "chash/chash.hpp"
#pragma comment(lib, "libchash.lib")

#include <stdio.h>

using namespace chash;

int main()
{
	if (IAlgorithm* SHA256 = createAlgorithm(EAlgorithm::SHA256)) {
		IDigest* Digest = SHA256->create();

		if (SHA256->init()) {
			SHA256->update((uint8_t*)"abcd", 4);
			SHA256->finalize(Digest);

			printf("%s\n", Digest->toHex().c_str());
		}
	}
}