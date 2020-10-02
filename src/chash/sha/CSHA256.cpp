#include "CSHA256.hpp"
#include <string.h>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))  
#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32 - (n))))  

#define S0(x) (ROTATE_RIGHT(x, 2) ^ ROTATE_RIGHT(x,13) ^ ROTATE_RIGHT(x, 22))  
#define S1(x) (ROTATE_RIGHT(x, 6) ^ ROTATE_RIGHT(x,11) ^ ROTATE_RIGHT(x, 25))  
#define s0(x) (ROTATE_RIGHT(x, 7) ^ ROTATE_RIGHT(x,18) ^ (x >> 3))  
#define s1(x) (ROTATE_RIGHT(x,17) ^ ROTATE_RIGHT(x,19) ^ (x >> 10))  

#define __BLK0(i) (W[i] = data[i])  
#define __BLK2(i) (W[i&15] += s1(W[(i-2)&15]) + W[(i-7)&15] + s0(W[(i-15)&15]))  

#define __Ch(x,y,z) (z^(x&(y^z)))  
#define __Maj(x,y,z) ((x&y)|(z&(x|y)))  

#define R(a,b,c,d,e,f,g,h, i) \
    h += S1(e) + __Ch(e,f,g) + K[i+j] + (j?__BLK2(i):__BLK0(i)); \
    d += h; h += S0(a) + __Maj(a, b, c)

#define RX_8(i) \
    R(a, b, c, d, e, f, g, h, i); \
    R(h, a, b, c, d, e, f, g, i + 1); \
    R(g, h, a, b, c, d, e, f, i + 2); \
    R(f, g, h, a, b, c, d, e, i + 3); \
    R(e, f, g, h, a, b, c, d, i + 4); \
    R(d, e, f, g, h, a, b, c, i + 5); \
    R(c, d, e, f, g, h, a, b, i + 6); \
    R(b, c, d, e, f, g, h, a, i + 7)

namespace chash {

    const uint32_t CSHA256::K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    CSHA256::CSHA256()
        : IAlgorithm(EAlgorithm::SHA256), 
          _init(false), _count(0)
    {
        ::memset(_state, 0, sizeof(_state));
        ::memset(_buffer, 0, sizeof(_buffer));
    }

    bool CSHA256::init() {
        if (_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        _init = true;
        _state[0] = 0x6a09e667;
        _state[1] = 0xbb67ae85;
        _state[2] = 0x3c6ef372;
        _state[3] = 0xa54ff53a;
        _state[4] = 0x510e527f;
        _state[5] = 0x9b05688c;
        _state[6] = 0x1f83d9ab;
        _state[7] = 0x5be0cd19;
        _count = 0;

        ::memset(_buffer, 0, sizeof(_buffer));
        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CSHA256::update(const uint8_t* inBytes, size_t inSize) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        uint32_t pos = uint32_t(_count) & 0x3f;

        while (inSize) {
            _buffer[pos++] = *inBytes++;
            _count++; inSize--;

            if (pos >= 64) {
                pos = 0;
                flush();
            }
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    bool CSHA256::finalize(IDigest* outDigest) {
        if (!_init) {
            setError(EAlgorithmErrno::InvalidState);
            return false;
        }

        if (outDigest->size() < 32) {
            setError(EAlgorithmErrno::InvalidDigest);
            return false;
        }

        if (!outDigest) {
            _init = false;
            return true;
        }

        uint64_t lenBits = _count << 3;
        uint32_t pos = uint32_t(_count) & 0x3f;
        uint8_t* digest = outDigest->bytes();

        _init = false;
        _buffer[pos++] = 0x80;
        while (pos != (64 - 8)) {
            pos &= 0x3f;

            if (!pos) {
                flush();
            }

            _buffer[pos++] = 0;
        }

        for (uint32_t i = 0; i < 8; ++i) {
            _buffer[pos++] = uint8_t(lenBits >> 56);
            lenBits <<= 8;
        }

        flush();
        for (uint32_t i = 0; i < 8; ++i) {
            *digest++ = uint8_t(_state[i] >> 24);
            *digest++ = uint8_t(_state[i] >> 16);
            *digest++ = uint8_t(_state[i] >> 8);
            *digest++ = uint8_t(_state[i]);
        }

        setError(EAlgorithmErrno::Succeed);
        return true;
    }

    void CSHA256::transform(const uint32_t* data)
    {
        uint32_t W[16],
            a = _state[0], b = _state[1], c = _state[2],
            d = _state[3], e = _state[4], f = _state[5],
            g = _state[6], h = _state[7],
            j;

        for (j = 0; j < 64; j += 16) {
            RX_8(0); RX_8(8);
        }

        _state[0] += a; _state[1] += b; _state[2] += c;
        _state[3] += d; _state[4] += e; _state[5] += f;
        _state[6] += g; _state[7] += h;
    }

    void CSHA256::flush() {
        uint32_t block[16];

        for (int32_t i = 0; i < 16; ++i) {
            block[i] =
                (uint32_t(_buffer[i * 4    ]) << 24) |
                (uint32_t(_buffer[i * 4 + 1]) << 16) |
                (uint32_t(_buffer[i * 4 + 2]) << 8) |
                (uint32_t(_buffer[i * 4 + 3]));
        }

        transform(block);
    }
}