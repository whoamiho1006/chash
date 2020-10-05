#include "crc64.hpp"

namespace chash {
	crc64::crc64()
		: hash_function(algorithm::CRC64), _init(false)
	{
	}

    bool crc64::init() {
        if (_init) {
            return false;
        }

        _init = true;

		for (uint64_t i = 0; i < 256; ++i) {
			uint64_t c = (i << 56);

            for (uint32_t j = 0; j < 8; ++j) {
                if ((c & 0x8000000000000000ull) != 0) {
                    c = (c << 1) ^ POLY_NOMIAL;
                }

                else c <<= 1;
            }

            _table[i] = c;
        }

        _digest = INIT_VALUE;
        return true;
    }

	void crc64::update(const uint8_t* inBytes, size_t inSize) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
        }

        while (inSize--) {
			uint8_t pos = ((_digest >> 56) ^ *inBytes++);
			_digest = uint64_t((_digest << 8) ^ _table[pos]);
        }
    }

	void crc64::finalize(digest_t& outDigest) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
		}

		_digest ^= FINAL_XOR;
		outDigest.push_back(uint8_t(_digest >> 56));
		outDigest.push_back(uint8_t(_digest >> 48));
		outDigest.push_back(uint8_t(_digest >> 40));
		outDigest.push_back(uint8_t(_digest >> 32));
		outDigest.push_back(uint8_t(_digest >> 24));
		outDigest.push_back(uint8_t(_digest >> 16));
		outDigest.push_back(uint8_t(_digest >> 8));
		outDigest.push_back(uint8_t(_digest));

		_init = false;
	}
}