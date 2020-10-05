#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int crc32()
{
	if (auto CRC32 = create(algorithm::CRC32)) {
		digest_t Digest;

		if (CRC32->init()) {
			CRC32->update((uint8_t*)"abcd", 4);
			CRC32->finalize(Digest);

			printf("CRC32(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}