#include "ripemd160.hpp"
#include <string.h>

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define G(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define H(x, y, z) (((x) | ~(y)) ^ (z))
#define I(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define J(x, y, z) ((x) ^ ((y) | ~(z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, e, x, s) a += F(b, c, d) + (x), a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define GG(a, b, c, d, e, x, s) a += G(b, c, d) + (x) + 0x5A827999, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define HH(a, b, c, d, e, x, s) a += H(b, c, d) + (x) + 0x6ED9EBA1, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define II(a, b, c, d, e, x, s) a += I(b, c, d) + (x) + 0x8F1BBCDC, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define JJ(a, b, c, d, e, x, s) a += J(b, c, d) + (x) + 0xA953FD4E, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)

#define FFF(a, b, c, d, e, x, s) a += F(b, c, d) + (x), a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define GGG(a, b, c, d, e, x, s) a += G(b, c, d) + (x) + 0x7A6D76E9, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define HHH(a, b, c, d, e, x, s) a += H(b, c, d) + (x) + 0x6D703EF3, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define III(a, b, c, d, e, x, s) a += I(b, c, d) + (x) + 0x5C4DD124, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)
#define JJJ(a, b, c, d, e, x, s) a += J(b, c, d) + (x) + 0x50A28BE6, a = ROTATE_LEFT(a, s) + (e), c = ROTATE_LEFT(c, 10)

namespace chash {
    const uint8_t ripemd160::PADDING[64] = { 0x80, 0, };

	ripemd160::ripemd160()
		: hash_function(algorithm::RipeMD160), _init(false), _count(0)
	{
	}

	bool ripemd160::init() {
        if (_init) {
            return false;
        }

        _init = true;
        _state[0] = 0x67452301;
        _state[1] = 0xEFCDAB89;
        _state[2] = 0x98BADCFE;
        _state[3] = 0x10325476;
        _state[4] = 0xC3D2E1F0;
        _count = 0;

        ::memset(_buffer, 0, sizeof(_buffer));
		return true;
	}

    void ripemd160::update(const uint8_t* inBytes, size_t inSize) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
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
    }
	
	void ripemd160::finalize(digest_t& outDigest) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
		}
		
		updateFinal();
		outDigest.reserve(20);

		for (int32_t i = 0, j = 0; j < 20; ++i, j += 4) {
			outDigest.push_back(uint8_t(_state[i]));
			outDigest.push_back(uint8_t(_state[i] >> 8));
			outDigest.push_back(uint8_t(_state[i] >> 16));
			outDigest.push_back(uint8_t(_state[i] >> 24));
		}

		_init = false;
	}

	void ripemd160::updateFinal()
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

	void ripemd160::flush() {
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

    void ripemd160::transform(const uint32_t* x) {
        //Initialize the working registers
        uint32_t aa = _state[0];
        uint32_t bb = _state[1];
        uint32_t cc = _state[2];
        uint32_t dd = _state[3];
        uint32_t ee = _state[4];
        uint32_t aaa = _state[0];
        uint32_t bbb = _state[1];
        uint32_t ccc = _state[2];
        uint32_t ddd = _state[3];
        uint32_t eee = _state[4];

        //Round 1
        FF(aa, bb, cc, dd, ee, x[0], 11);
        FF(ee, aa, bb, cc, dd, x[1], 14);
        FF(dd, ee, aa, bb, cc, x[2], 15);
        FF(cc, dd, ee, aa, bb, x[3], 12);
        FF(bb, cc, dd, ee, aa, x[4], 5);
        FF(aa, bb, cc, dd, ee, x[5], 8);
        FF(ee, aa, bb, cc, dd, x[6], 7);
        FF(dd, ee, aa, bb, cc, x[7], 9);
        FF(cc, dd, ee, aa, bb, x[8], 11);
        FF(bb, cc, dd, ee, aa, x[9], 13);
        FF(aa, bb, cc, dd, ee, x[10], 14);
        FF(ee, aa, bb, cc, dd, x[11], 15);
        FF(dd, ee, aa, bb, cc, x[12], 6);
        FF(cc, dd, ee, aa, bb, x[13], 7);
        FF(bb, cc, dd, ee, aa, x[14], 9);
        FF(aa, bb, cc, dd, ee, x[15], 8);

        //Round 2
        GG(ee, aa, bb, cc, dd, x[7], 7);
        GG(dd, ee, aa, bb, cc, x[4], 6);
        GG(cc, dd, ee, aa, bb, x[13], 8);
        GG(bb, cc, dd, ee, aa, x[1], 13);
        GG(aa, bb, cc, dd, ee, x[10], 11);
        GG(ee, aa, bb, cc, dd, x[6], 9);
        GG(dd, ee, aa, bb, cc, x[15], 7);
        GG(cc, dd, ee, aa, bb, x[3], 15);
        GG(bb, cc, dd, ee, aa, x[12], 7);
        GG(aa, bb, cc, dd, ee, x[0], 12);
        GG(ee, aa, bb, cc, dd, x[9], 15);
        GG(dd, ee, aa, bb, cc, x[5], 9);
        GG(cc, dd, ee, aa, bb, x[2], 11);
        GG(bb, cc, dd, ee, aa, x[14], 7);
        GG(aa, bb, cc, dd, ee, x[11], 13);
        GG(ee, aa, bb, cc, dd, x[8], 12);

        //Round 3
        HH(dd, ee, aa, bb, cc, x[3], 11);
        HH(cc, dd, ee, aa, bb, x[10], 13);
        HH(bb, cc, dd, ee, aa, x[14], 6);
        HH(aa, bb, cc, dd, ee, x[4], 7);
        HH(ee, aa, bb, cc, dd, x[9], 14);
        HH(dd, ee, aa, bb, cc, x[15], 9);
        HH(cc, dd, ee, aa, bb, x[8], 13);
        HH(bb, cc, dd, ee, aa, x[1], 15);
        HH(aa, bb, cc, dd, ee, x[2], 14);
        HH(ee, aa, bb, cc, dd, x[7], 8);
        HH(dd, ee, aa, bb, cc, x[0], 13);
        HH(cc, dd, ee, aa, bb, x[6], 6);
        HH(bb, cc, dd, ee, aa, x[13], 5);
        HH(aa, bb, cc, dd, ee, x[11], 12);
        HH(ee, aa, bb, cc, dd, x[5], 7);
        HH(dd, ee, aa, bb, cc, x[12], 5);

        //Round 4
        II(cc, dd, ee, aa, bb, x[1], 11);
        II(bb, cc, dd, ee, aa, x[9], 12);
        II(aa, bb, cc, dd, ee, x[11], 14);
        II(ee, aa, bb, cc, dd, x[10], 15);
        II(dd, ee, aa, bb, cc, x[0], 14);
        II(cc, dd, ee, aa, bb, x[8], 15);
        II(bb, cc, dd, ee, aa, x[12], 9);
        II(aa, bb, cc, dd, ee, x[4], 8);
        II(ee, aa, bb, cc, dd, x[13], 9);
        II(dd, ee, aa, bb, cc, x[3], 14);
        II(cc, dd, ee, aa, bb, x[7], 5);
        II(bb, cc, dd, ee, aa, x[15], 6);
        II(aa, bb, cc, dd, ee, x[14], 8);
        II(ee, aa, bb, cc, dd, x[5], 6);
        II(dd, ee, aa, bb, cc, x[6], 5);
        II(cc, dd, ee, aa, bb, x[2], 12);

        //Round 5
        JJ(bb, cc, dd, ee, aa, x[4], 9);
        JJ(aa, bb, cc, dd, ee, x[0], 15);
        JJ(ee, aa, bb, cc, dd, x[5], 5);
        JJ(dd, ee, aa, bb, cc, x[9], 11);
        JJ(cc, dd, ee, aa, bb, x[7], 6);
        JJ(bb, cc, dd, ee, aa, x[12], 8);
        JJ(aa, bb, cc, dd, ee, x[2], 13);
        JJ(ee, aa, bb, cc, dd, x[10], 12);
        JJ(dd, ee, aa, bb, cc, x[14], 5);
        JJ(cc, dd, ee, aa, bb, x[1], 12);
        JJ(bb, cc, dd, ee, aa, x[3], 13);
        JJ(aa, bb, cc, dd, ee, x[8], 14);
        JJ(ee, aa, bb, cc, dd, x[11], 11);
        JJ(dd, ee, aa, bb, cc, x[6], 8);
        JJ(cc, dd, ee, aa, bb, x[15], 5);
        JJ(bb, cc, dd, ee, aa, x[13], 6);

        //Parallel round 1
        JJJ(aaa, bbb, ccc, ddd, eee, x[5], 8);
        JJJ(eee, aaa, bbb, ccc, ddd, x[14], 9);
        JJJ(ddd, eee, aaa, bbb, ccc, x[7], 9);
        JJJ(ccc, ddd, eee, aaa, bbb, x[0], 11);
        JJJ(bbb, ccc, ddd, eee, aaa, x[9], 13);
        JJJ(aaa, bbb, ccc, ddd, eee, x[2], 15);
        JJJ(eee, aaa, bbb, ccc, ddd, x[11], 15);
        JJJ(ddd, eee, aaa, bbb, ccc, x[4], 5);
        JJJ(ccc, ddd, eee, aaa, bbb, x[13], 7);
        JJJ(bbb, ccc, ddd, eee, aaa, x[6], 7);
        JJJ(aaa, bbb, ccc, ddd, eee, x[15], 8);
        JJJ(eee, aaa, bbb, ccc, ddd, x[8], 11);
        JJJ(ddd, eee, aaa, bbb, ccc, x[1], 14);
        JJJ(ccc, ddd, eee, aaa, bbb, x[10], 14);
        JJJ(bbb, ccc, ddd, eee, aaa, x[3], 12);
        JJJ(aaa, bbb, ccc, ddd, eee, x[12], 6);

        //Parallel round 2
        III(eee, aaa, bbb, ccc, ddd, x[6], 9);
        III(ddd, eee, aaa, bbb, ccc, x[11], 13);
        III(ccc, ddd, eee, aaa, bbb, x[3], 15);
        III(bbb, ccc, ddd, eee, aaa, x[7], 7);
        III(aaa, bbb, ccc, ddd, eee, x[0], 12);
        III(eee, aaa, bbb, ccc, ddd, x[13], 8);
        III(ddd, eee, aaa, bbb, ccc, x[5], 9);
        III(ccc, ddd, eee, aaa, bbb, x[10], 11);
        III(bbb, ccc, ddd, eee, aaa, x[14], 7);
        III(aaa, bbb, ccc, ddd, eee, x[15], 7);
        III(eee, aaa, bbb, ccc, ddd, x[8], 12);
        III(ddd, eee, aaa, bbb, ccc, x[12], 7);
        III(ccc, ddd, eee, aaa, bbb, x[4], 6);
        III(bbb, ccc, ddd, eee, aaa, x[9], 15);
        III(aaa, bbb, ccc, ddd, eee, x[1], 13);
        III(eee, aaa, bbb, ccc, ddd, x[2], 11);

        //Parallel round 3
        HHH(ddd, eee, aaa, bbb, ccc, x[15], 9);
        HHH(ccc, ddd, eee, aaa, bbb, x[5], 7);
        HHH(bbb, ccc, ddd, eee, aaa, x[1], 15);
        HHH(aaa, bbb, ccc, ddd, eee, x[3], 11);
        HHH(eee, aaa, bbb, ccc, ddd, x[7], 8);
        HHH(ddd, eee, aaa, bbb, ccc, x[14], 6);
        HHH(ccc, ddd, eee, aaa, bbb, x[6], 6);
        HHH(bbb, ccc, ddd, eee, aaa, x[9], 14);
        HHH(aaa, bbb, ccc, ddd, eee, x[11], 12);
        HHH(eee, aaa, bbb, ccc, ddd, x[8], 13);
        HHH(ddd, eee, aaa, bbb, ccc, x[12], 5);
        HHH(ccc, ddd, eee, aaa, bbb, x[2], 14);
        HHH(bbb, ccc, ddd, eee, aaa, x[10], 13);
        HHH(aaa, bbb, ccc, ddd, eee, x[0], 13);
        HHH(eee, aaa, bbb, ccc, ddd, x[4], 7);
        HHH(ddd, eee, aaa, bbb, ccc, x[13], 5);

        //Parallel round 4
        GGG(ccc, ddd, eee, aaa, bbb, x[8], 15);
        GGG(bbb, ccc, ddd, eee, aaa, x[6], 5);
        GGG(aaa, bbb, ccc, ddd, eee, x[4], 8);
        GGG(eee, aaa, bbb, ccc, ddd, x[1], 11);
        GGG(ddd, eee, aaa, bbb, ccc, x[3], 14);
        GGG(ccc, ddd, eee, aaa, bbb, x[11], 14);
        GGG(bbb, ccc, ddd, eee, aaa, x[15], 6);
        GGG(aaa, bbb, ccc, ddd, eee, x[0], 14);
        GGG(eee, aaa, bbb, ccc, ddd, x[5], 6);
        GGG(ddd, eee, aaa, bbb, ccc, x[12], 9);
        GGG(ccc, ddd, eee, aaa, bbb, x[2], 12);
        GGG(bbb, ccc, ddd, eee, aaa, x[13], 9);
        GGG(aaa, bbb, ccc, ddd, eee, x[9], 12);
        GGG(eee, aaa, bbb, ccc, ddd, x[7], 5);
        GGG(ddd, eee, aaa, bbb, ccc, x[10], 15);
        GGG(ccc, ddd, eee, aaa, bbb, x[14], 8);

        //Parallel round 5
        FFF(bbb, ccc, ddd, eee, aaa, x[12], 8);
        FFF(aaa, bbb, ccc, ddd, eee, x[15], 5);
        FFF(eee, aaa, bbb, ccc, ddd, x[10], 12);
        FFF(ddd, eee, aaa, bbb, ccc, x[4], 9);
        FFF(ccc, ddd, eee, aaa, bbb, x[1], 12);
        FFF(bbb, ccc, ddd, eee, aaa, x[5], 5);
        FFF(aaa, bbb, ccc, ddd, eee, x[8], 14);
        FFF(eee, aaa, bbb, ccc, ddd, x[7], 6);
        FFF(ddd, eee, aaa, bbb, ccc, x[6], 8);
        FFF(ccc, ddd, eee, aaa, bbb, x[2], 13);
        FFF(bbb, ccc, ddd, eee, aaa, x[13], 6);
        FFF(aaa, bbb, ccc, ddd, eee, x[14], 5);
        FFF(eee, aaa, bbb, ccc, ddd, x[0], 15);
        FFF(ddd, eee, aaa, bbb, ccc, x[3], 13);
        FFF(ccc, ddd, eee, aaa, bbb, x[9], 11);
        FFF(bbb, ccc, ddd, eee, aaa, x[11], 11);

        //Combine results
        ddd = _state[1] + cc + ddd;
        _state[1] = _state[2] + dd + eee;
        _state[2] = _state[3] + ee + aaa;
        _state[3] = _state[4] + aa + bbb;
        _state[4] = _state[0] + bb + ccc;
        _state[0] = ddd;
    }


}