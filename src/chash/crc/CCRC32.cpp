#include "CCRC32.hpp"

namespace chash {
    CCRC32::CCRC32()
        : IAlgorithm(EAlgorithm::CRC32), _init(0)
    {
    }

    bool CCRC32::init() {
        if (_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        _init = true;
        for (int32_t i = 0; i < 256; i++)
        {
            uint32_t c = uint32_t(i << 24);

            for (uint8_t b = 0; b < 8; b++)
            {
                if ((c & 0x80000000) != 0)
                    c = (c << 1) ^ POLY_NOMIAL;

                else
                    c <<= 1;
            }

            _table[i] = c;
        }

        _digest = INIT_VALUE;
        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC32::update(const uint8_t* inBytes, size_t inSize) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        while (inSize--) {
            uint8_t pos = (_digest ^ (*inBytes++ << 24)) >> 24;
            _digest = uint32_t((_digest << 8) ^ _table[pos]);
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

	bool CCRC32::finalize(CDigest& outDigest) {
		if (!_init) {
			setError(EAlgorithmErrno::InvalidState);
			return false;
		}

		_digest ^= FINAL_XOR;
		outDigest.push_back(uint8_t(_digest >> 24));
		outDigest.push_back(uint8_t(_digest >> 16));
		outDigest.push_back(uint8_t(_digest >> 8));
		outDigest.push_back(uint8_t(_digest));

		_init = false;
		setError(EAlgorithmErrno::Succeed);
		return true;
	}
}