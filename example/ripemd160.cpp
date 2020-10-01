
#include "chash/chash.hpp"
#pragma comment(lib, "libchash.lib")

#include <stdio.h>

using namespace chash;

int main()
{
	if (IAlgorithm* RipeMD160 = createAlgorithm(EAlgorithm::RipeMD160)) {
		IDigest* Digest = RipeMD160->create();

		if (RipeMD160->init()) {
			RipeMD160->update((uint8_t*)"abcd", 4);
			RipeMD160->finalize(Digest);

			printf("%s\n", Digest->toHex().c_str());
		}
	}
}