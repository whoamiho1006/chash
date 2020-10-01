﻿
#include "chash/chash.hpp"
#pragma comment(lib, "libchash.lib")

#include <stdio.h>

using namespace chash;

int main()
{
	if (IAlgorithm* MD5 = createAlgorithm(EAlgorithm::MD5)) {
		IDigest* Digest = MD5->create();

		if (MD5->init()) {
			MD5->update((uint8_t*)"abcd", 4);
			MD5->finalize(Digest);

			printf("%s\n", Digest->toHex().c_str());
		}
	}
}