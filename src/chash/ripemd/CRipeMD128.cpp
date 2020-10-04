#include "CRipeMD128.hpp"
#include <string.h>

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define G(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define H(x, y, z) (((x) | ~(y)) ^ (z))
#define I(x, y, z) (((x) & (z)) | ((y) & ~(z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s) a += F(b, c, d) + (x), a = ROTATE_LEFT(a, s)
#define GG(a, b, c, d, x, s) a += G(b, c, d) + (x) + 0x5A827999, a = ROTATE_LEFT(a, s)
#define HH(a, b, c, d, x, s) a += H(b, c, d) + (x) + 0x6ED9EBA1, a = ROTATE_LEFT(a, s)
#define II(a, b, c, d, x, s) a += I(b, c, d) + (x) + 0x8F1BBCDC, a = ROTATE_LEFT(a, s)

#define FFF(a, b, c, d, x, s) a += F(b, c, d) + (x), a = ROTATE_LEFT(a, s)
#define GGG(a, b, c, d, x, s) a += G(b, c, d) + (x) + 0x6D703EF3, a = ROTATE_LEFT(a, s)
#define HHH(a, b, c, d, x, s) a += H(b, c, d) + (x) + 0x5C4DD124, a = ROTATE_LEFT(a, s)
#define III(a, b, c, d, x, s) a += I(b, c, d) + (x) + 0x50A28BE6, a = ROTATE_LEFT(a, s)

namespace chash {
	const uint8_t CRipeMD128::PADDING[64] = { 0x80, 0, };

	CRipeMD128::CRipeMD128()
		: IAlgorithm(EAlgorithm::RipeMD128), _init(false), _count(0)
	{
	}

	bool CRipeMD128::init() {
		if (_init) {
			setError(EAlgorithmErrno::InvalidState);
			return false;
		}

		_init = true;
		_state[0] = 0x67452301;
		_state[1] = 0xEFCDAB89;
		_state[2] = 0x98BADCFE;
		_state[3] = 0x10325476;
		_count = 0;

		::memset(_buffer, 0, sizeof(_buffer));
		setError(EAlgorithmErrno::Succeed);
		return true;
	}

	bool CRipeMD128::update(const uint8_t* inBytes, size_t inSize) {
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

	bool CRipeMD128::finalize(CDigest& outDigest) {
		if (!_init) {
			setError(EAlgorithmErrno::InvalidState);
			return false;
		}

		updateFinal();
		outDigest.reserve(16);

		for (int32_t i = 0, j = 0; j < 16; ++i, j += 4) {
			outDigest.push_back(uint8_t(_state[i]));
			outDigest.push_back(uint8_t(_state[i] >> 8));
			outDigest.push_back(uint8_t(_state[i] >> 16));
			outDigest.push_back(uint8_t(_state[i] >> 24));
		}

		_init = false;
		setError(EAlgorithmErrno::Succeed);
		return true;
	}

	void CRipeMD128::updateFinal()
	{
		uint8_t lenBits[8];
		uint64_t length = _count << 3;
		uint32_t index = uint32_t(_count & 0x3f),
			pads = index < 56 ? 56 - index : 120 - index;

		lenBits[0] = uint8_t(length);
		lenBits[1] = uint8_t(length >> 8);
		lenBits[2] = uint8_t(length >> 16);
		lenBits[3] = uint8_t(length >> 24);
		lenBits[4] = uint8_t(length >> 32);
		lenBits[5] = uint8_t(length >> 40);
		lenBits[6] = uint8_t(length >> 48);
		lenBits[7] = uint8_t(length >> 56);

		update(PADDING, pads);
		update(lenBits, 8);
	}

	void CRipeMD128::flush() {
        uint32_t block[16];

        for (int32_t i = 0; i < 16; ++i) {
            block[i] =
                (uint32_t(_buffer[i * 4 + 3]) << 24) |
                (uint32_t(_buffer[i * 4 + 2]) << 16) |
                (uint32_t(_buffer[i * 4 + 1]) << 8) |
                (uint32_t(_buffer[i * 4 + 0]));
        }

        transform(block);
	}

	void CRipeMD128::transform(const uint32_t* x)
	{
        //Initialize the working registers
        uint32_t aa = _state[0];
        uint32_t bb = _state[1];
        uint32_t cc = _state[2];
        uint32_t dd = _state[3];
        uint32_t aaa = _state[0];
        uint32_t bbb = _state[1];
        uint32_t ccc = _state[2];
        uint32_t ddd = _state[3];

        //Round 1
        FF(aa, bb, cc, dd, x[0], 11);
        FF(dd, aa, bb, cc, x[1], 14);
        FF(cc, dd, aa, bb, x[2], 15);
        FF(bb, cc, dd, aa, x[3], 12);
        FF(aa, bb, cc, dd, x[4], 5);
        FF(dd, aa, bb, cc, x[5], 8);
        FF(cc, dd, aa, bb, x[6], 7);
        FF(bb, cc, dd, aa, x[7], 9);
        FF(aa, bb, cc, dd, x[8], 11);
        FF(dd, aa, bb, cc, x[9], 13);
        FF(cc, dd, aa, bb, x[10], 14);
        FF(bb, cc, dd, aa, x[11], 15);
        FF(aa, bb, cc, dd, x[12], 6);
        FF(dd, aa, bb, cc, x[13], 7);
        FF(cc, dd, aa, bb, x[14], 9);
        FF(bb, cc, dd, aa, x[15], 8);

        //Round 2
        GG(aa, bb, cc, dd, x[7], 7);
        GG(dd, aa, bb, cc, x[4], 6);
        GG(cc, dd, aa, bb, x[13], 8);
        GG(bb, cc, dd, aa, x[1], 13);
        GG(aa, bb, cc, dd, x[10], 11);
        GG(dd, aa, bb, cc, x[6], 9);
        GG(cc, dd, aa, bb, x[15], 7);
        GG(bb, cc, dd, aa, x[3], 15);
        GG(aa, bb, cc, dd, x[12], 7);
        GG(dd, aa, bb, cc, x[0], 12);
        GG(cc, dd, aa, bb, x[9], 15);
        GG(bb, cc, dd, aa, x[5], 9);
        GG(aa, bb, cc, dd, x[2], 11);
        GG(dd, aa, bb, cc, x[14], 7);
        GG(cc, dd, aa, bb, x[11], 13);
        GG(bb, cc, dd, aa, x[8], 12);

        //Round 3
        HH(aa, bb, cc, dd, x[3], 11);
        HH(dd, aa, bb, cc, x[10], 13);
        HH(cc, dd, aa, bb, x[14], 6);
        HH(bb, cc, dd, aa, x[4], 7);
        HH(aa, bb, cc, dd, x[9], 14);
        HH(dd, aa, bb, cc, x[15], 9);
        HH(cc, dd, aa, bb, x[8], 13);
        HH(bb, cc, dd, aa, x[1], 15);
        HH(aa, bb, cc, dd, x[2], 14);
        HH(dd, aa, bb, cc, x[7], 8);
        HH(cc, dd, aa, bb, x[0], 13);
        HH(bb, cc, dd, aa, x[6], 6);
        HH(aa, bb, cc, dd, x[13], 5);
        HH(dd, aa, bb, cc, x[11], 12);
        HH(cc, dd, aa, bb, x[5], 7);
        HH(bb, cc, dd, aa, x[12], 5);

        //Round 4
        II(aa, bb, cc, dd, x[1], 11);
        II(dd, aa, bb, cc, x[9], 12);
        II(cc, dd, aa, bb, x[11], 14);
        II(bb, cc, dd, aa, x[10], 15);
        II(aa, bb, cc, dd, x[0], 14);
        II(dd, aa, bb, cc, x[8], 15);
        II(cc, dd, aa, bb, x[12], 9);
        II(bb, cc, dd, aa, x[4], 8);
        II(aa, bb, cc, dd, x[13], 9);
        II(dd, aa, bb, cc, x[3], 14);
        II(cc, dd, aa, bb, x[7], 5);
        II(bb, cc, dd, aa, x[15], 6);
        II(aa, bb, cc, dd, x[14], 8);
        II(dd, aa, bb, cc, x[5], 6);
        II(cc, dd, aa, bb, x[6], 5);
        II(bb, cc, dd, aa, x[2], 12);

        //Parallel round 1
        III(aaa, bbb, ccc, ddd, x[5], 8);
        III(ddd, aaa, bbb, ccc, x[14], 9);
        III(ccc, ddd, aaa, bbb, x[7], 9);
        III(bbb, ccc, ddd, aaa, x[0], 11);
        III(aaa, bbb, ccc, ddd, x[9], 13);
        III(ddd, aaa, bbb, ccc, x[2], 15);
        III(ccc, ddd, aaa, bbb, x[11], 15);
        III(bbb, ccc, ddd, aaa, x[4], 5);
        III(aaa, bbb, ccc, ddd, x[13], 7);
        III(ddd, aaa, bbb, ccc, x[6], 7);
        III(ccc, ddd, aaa, bbb, x[15], 8);
        III(bbb, ccc, ddd, aaa, x[8], 11);
        III(aaa, bbb, ccc, ddd, x[1], 14);
        III(ddd, aaa, bbb, ccc, x[10], 14);
        III(ccc, ddd, aaa, bbb, x[3], 12);
        III(bbb, ccc, ddd, aaa, x[12], 6);

        //Parallel round 2
        HHH(aaa, bbb, ccc, ddd, x[6], 9);
        HHH(ddd, aaa, bbb, ccc, x[11], 13);
        HHH(ccc, ddd, aaa, bbb, x[3], 15);
        HHH(bbb, ccc, ddd, aaa, x[7], 7);
        HHH(aaa, bbb, ccc, ddd, x[0], 12);
        HHH(ddd, aaa, bbb, ccc, x[13], 8);
        HHH(ccc, ddd, aaa, bbb, x[5], 9);
        HHH(bbb, ccc, ddd, aaa, x[10], 11);
        HHH(aaa, bbb, ccc, ddd, x[14], 7);
        HHH(ddd, aaa, bbb, ccc, x[15], 7);
        HHH(ccc, ddd, aaa, bbb, x[8], 12);
        HHH(bbb, ccc, ddd, aaa, x[12], 7);
        HHH(aaa, bbb, ccc, ddd, x[4], 6);
        HHH(ddd, aaa, bbb, ccc, x[9], 15);
        HHH(ccc, ddd, aaa, bbb, x[1], 13);
        HHH(bbb, ccc, ddd, aaa, x[2], 11);

        //Parallel round 3
        GGG(aaa, bbb, ccc, ddd, x[15], 9);
        GGG(ddd, aaa, bbb, ccc, x[5], 7);
        GGG(ccc, ddd, aaa, bbb, x[1], 15);
        GGG(bbb, ccc, ddd, aaa, x[3], 11);
        GGG(aaa, bbb, ccc, ddd, x[7], 8);
        GGG(ddd, aaa, bbb, ccc, x[14], 6);
        GGG(ccc, ddd, aaa, bbb, x[6], 6);
        GGG(bbb, ccc, ddd, aaa, x[9], 14);
        GGG(aaa, bbb, ccc, ddd, x[11], 12);
        GGG(ddd, aaa, bbb, ccc, x[8], 13);
        GGG(ccc, ddd, aaa, bbb, x[12], 5);
        GGG(bbb, ccc, ddd, aaa, x[2], 14);
        GGG(aaa, bbb, ccc, ddd, x[10], 13);
        GGG(ddd, aaa, bbb, ccc, x[0], 13);
        GGG(ccc, ddd, aaa, bbb, x[4], 7);
        GGG(bbb, ccc, ddd, aaa, x[13], 5);

        //Parallel round 4
        FFF(aaa, bbb, ccc, ddd, x[8], 15);
        FFF(ddd, aaa, bbb, ccc, x[6], 5);
        FFF(ccc, ddd, aaa, bbb, x[4], 8);
        FFF(bbb, ccc, ddd, aaa, x[1], 11);
        FFF(aaa, bbb, ccc, ddd, x[3], 14);
        FFF(ddd, aaa, bbb, ccc, x[11], 14);
        FFF(ccc, ddd, aaa, bbb, x[15], 6);
        FFF(bbb, ccc, ddd, aaa, x[0], 14);
        FFF(aaa, bbb, ccc, ddd, x[5], 6);
        FFF(ddd, aaa, bbb, ccc, x[12], 9);
        FFF(ccc, ddd, aaa, bbb, x[2], 12);
        FFF(bbb, ccc, ddd, aaa, x[13], 9);
        FFF(aaa, bbb, ccc, ddd, x[9], 12);
        FFF(ddd, aaa, bbb, ccc, x[7], 5);
        FFF(ccc, ddd, aaa, bbb, x[10], 15);
        FFF(bbb, ccc, ddd, aaa, x[14], 8);

        //Combine results
        ddd = _state[1] + cc + ddd;
        _state[1] = _state[2] + dd + aaa;
        _state[2] = _state[3] + aa + bbb;
        _state[3] = _state[0] + bb + ccc;
        _state[0] = ddd;
	}
}