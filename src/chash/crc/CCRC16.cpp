#include "CCRC16.hpp"

namespace chash {

    const uint16_t CCRC16::TABLE[256] = {

    };

    CCRC16::CCRC16()
        : IAlgorithm(EAlgorithm::CRC16),
        _init(0), _digest(0)
    {
    }

    bool CCRC16::init() {
        if (_init) {
            setError(EAlgorithmErrno::InvalidState);
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
        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC16::update(const uint8_t* inBytes, size_t inSize) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        while (inSize--) {
            uint8_t pos = (_digest >> 8) ^ *inBytes++;
            _digest = uint16_t((_digest << 8) ^ _table[pos]);
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CCRC16::finalize(IDigest* outDigest) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        if (outDigest->size() < sizeof(uint16_t)) {
            setError(EAlgorithmErrno::InvalidDigest);
            return false;
        }

        if (!outDigest) {
            _init = false;
            return true;
        }

        uint8_t* digest = outDigest->bytes();

        *digest++ = uint8_t(_digest >> 8);
        *digest++ = uint8_t(_digest);

        _init = false;
        setError(EAlgorithmErrno::Succeed);
        return true;
    }
}