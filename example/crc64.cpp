#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int crc64()
{
	if (auto CRC64 = create(algorithm::CRC64)) {
		digest_t Digest;

		if (CRC64->init()) {
			CRC64->update((uint8_t*)"abcd", 4);
			CRC64->finalize(Digest);

			printf("CRC64(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}