#include "chash/chash.hpp"
#include <stdio.h>

using namespace chash;

int crc16()
{
	if (auto CRC16 = create(algorithm::CRC16)) {
		digest_t Digest;

		if (CRC16->init()) {
			CRC16->update((uint8_t*)"abcd", 4);
			CRC16->finalize(Digest);

			printf("CRC16(abcd): %s\n", to_hex(Digest).c_str());
			return 0;
		}
	}

	return -1;
}