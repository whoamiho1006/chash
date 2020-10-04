#include "CCRC64.hpp"

namespace chash {
	CCRC64::CCRC64()
		: IAlgorithm(EAlgorithm::CRC64), _init(false)
	{
	}

    bool CCRC64::init() {
        if (_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        uint64_t temp = 0;
        _init = true;

        for (uint64_t i = 0; i < 256; ++i) {
            temp = (i << 56);

            for (uint32_t j = 0; j < 8; ++j) {
                if (temp & 0x8000000000000000ull) {
                    temp = (temp << 1) ^ POLY_NOMIAL;
                }

                else temp <<= 1;
            }

            _table[i] = temp;
        }

        _digest = INIT_VALUE;
        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC64::update(const uint8_t* inBytes, size_t inSize) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        while (inSize--) {
            uint32_t i = ((_digest >> 56) ^ *inBytes++) & 0xff;
            _digest = (_digest << 8) ^ _table[i];
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

	bool CCRC64::finalize(CDigest& outDigest) {
		if (!_init) {
			setError(EAlgorithmErrno::InvalidState);
			return false;
		}

		outDigest.push_back(uint8_t(_digest >> 56));
		outDigest.push_back(uint8_t(_digest >> 48));
		outDigest.push_back(uint8_t(_digest >> 40));
		outDigest.push_back(uint8_t(_digest >> 32));
		outDigest.push_back(uint8_t(_digest >> 24));
		outDigest.push_back(uint8_t(_digest >> 16));
		outDigest.push_back(uint8_t(_digest >> 8));
		outDigest.push_back(uint8_t(_digest));

		_init = false;
		setError(EAlgorithmErrno::Succeed);
		return true;
	}
}