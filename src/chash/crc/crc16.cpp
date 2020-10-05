#include "crc16.hpp"

namespace chash {

    const uint16_t crc16::TABLE[256] = {

    };

    crc16::crc16()
        : hash_function(algorithm::CRC16),
        _init(0), _digest(0)
    {
    }

    bool crc16::init() {
        if (_init) {
            return false;
        }

        _init = true;
        for (int32_t i = 0; i < 256; i++) {
            uint16_t c = uint16_t(i << 8);

            for (uint8_t b = 0; b < 8; b++)
            {
                if ((c & 0x8000) != 0)
                    c = (c << 1) ^ POLY_NOMIAL;

                else
                    c <<= 1;
            }

            _table[i] = c;
        }

        _digest = INIT_VALUE;
        return true;
    }

    void crc16::update(const uint8_t* inBytes, size_t inSize) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
        }

        while (inSize--) {
            uint8_t pos = (_digest >> 8) ^ *inBytes++;
            _digest = uint16_t((_digest << 8) ^ _table[pos]);
        }
    }

	void crc16::finalize(digest_t& outDigest) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
		}

		_digest ^= FINAL_XOR;
		outDigest.push_back(uint8_t(_digest >> 8));
		outDigest.push_back(uint8_t(_digest));

		_init = false;
	}
}