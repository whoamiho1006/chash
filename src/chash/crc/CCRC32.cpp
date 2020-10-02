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

        uint32_t temp = 0;
        _init = true;

        for (uint32_t i = 0; i < 256; ++i) {
            temp = (i << 24);

            for (uint32_t j = 0; j < 8; ++j) {
                if (temp & 0x80000000u) {
                    temp = (temp << 1) ^ POLY_NOMIAL;
                }

                else temp <<= 1;
            }

            _table[i] = temp;
        }

        _digest = 0;
        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC32::update(const uint8_t* inBytes, size_t inSize) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        while (inSize--) {
            uint32_t i = ((_digest >> 24) ^ *inBytes++) & 0xff;
            _digest = (_digest << 8) ^ _table[i];
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC32::finalize(IDigest* outDigest) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        if (outDigest->size() < sizeof(uint32_t)) {
            setError(EAlgorithmErrno::InvalidDigest);
            return false;
        }

        if (!outDigest) {
            _init = false;
            return true;
        }

        uint8_t* digest = outDigest->bytes();

        *digest++ = uint8_t(_digest >> 24);
        *digest++ = uint8_t(_digest >> 16);
        *digest++ = uint8_t(_digest >> 8);
        *digest++ = uint8_t(_digest);

        _init = false;
        setError(EAlgorithmErrno::Succeed);
        return true;
    }
}