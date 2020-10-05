#include "CMD5.hpp"

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}

namespace chash {
	const uint8_t CMD5::PADDING[64] = { 0x80, 0, };

	CMD5::CMD5()
		: IAlgorithm(EAlgorithm::MD5), _init(false), _count(0)
	{
	}

	bool CMD5::init()
	{
		if (_init) {
			return false;
		}

		_init = true;

		_state[0] = 0x67452301;
		_state[1] = 0xefcdab89;
		_state[2] = 0x98badcfe;
		_state[3] = 0x10325476;

		_count = 0;
		return true;
	}

	void CMD5::update(const uint8_t* inBytes, size_t inSize) {
		if (!_init) {
			throw new CInvalidStateError("Can't perform anything for non-initiated algorithm!");
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

	void CMD5::finalize(CDigest& outDigest) {
		if (!_init) {
			throw new CInvalidStateError("Can't perform anything for non-initiated algorithm!");
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
	}

	void CMD5::updateFinal() {
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

	void CMD5::flush() {
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

	void CMD5::transform(const uint32_t* data) {
		uint32_t a = _state[0], b = _state[1], c = _state[2], d = _state[3];

		/* Round 1 */
		FF(a, b, c, d, data[0], S11, 0xd76aa478); /* 1 */
		FF(d, a, b, c, data[1], S12, 0xe8c7b756); /* 2 */
		FF(c, d, a, b, data[2], S13, 0x242070db); /* 3 */
		FF(b, c, d, a, data[3], S14, 0xc1bdceee); /* 4 */
		FF(a, b, c, d, data[4], S11, 0xf57c0faf); /* 5 */
		FF(d, a, b, c, data[5], S12, 0x4787c62a); /* 6 */
		FF(c, d, a, b, data[6], S13, 0xa8304613); /* 7 */
		FF(b, c, d, a, data[7], S14, 0xfd469501); /* 8 */
		FF(a, b, c, d, data[8], S11, 0x698098d8); /* 9 */
		FF(d, a, b, c, data[9], S12, 0x8b44f7af); /* 10 */
		FF(c, d, a, b, data[10], S13, 0xffff5bb1); /* 11 */
		FF(b, c, d, a, data[11], S14, 0x895cd7be); /* 12 */
		FF(a, b, c, d, data[12], S11, 0x6b901122); /* 13 */
		FF(d, a, b, c, data[13], S12, 0xfd987193); /* 14 */
		FF(c, d, a, b, data[14], S13, 0xa679438e); /* 15 */
		FF(b, c, d, a, data[15], S14, 0x49b40821); /* 16 */

		/* Round 2 */
		GG(a, b, c, d, data[1], S21, 0xf61e2562); /* 17 */
		GG(d, a, b, c, data[6], S22, 0xc040b340); /* 18 */
		GG(c, d, a, b, data[11], S23, 0x265e5a51); /* 19 */
		GG(b, c, d, a, data[0], S24, 0xe9b6c7aa); /* 20 */
		GG(a, b, c, d, data[5], S21, 0xd62f105d); /* 21 */
		GG(d, a, b, c, data[10], S22, 0x2441453); /* 22 */
		GG(c, d, a, b, data[15], S23, 0xd8a1e681); /* 23 */
		GG(b, c, d, a, data[4], S24, 0xe7d3fbc8); /* 24 */
		GG(a, b, c, d, data[9], S21, 0x21e1cde6); /* 25 */
		GG(d, a, b, c, data[14], S22, 0xc33707d6); /* 26 */
		GG(c, d, a, b, data[3], S23, 0xf4d50d87); /* 27 */
		GG(b, c, d, a, data[8], S24, 0x455a14ed); /* 28 */
		GG(a, b, c, d, data[13], S21, 0xa9e3e905); /* 29 */
		GG(d, a, b, c, data[2], S22, 0xfcefa3f8); /* 30 */
		GG(c, d, a, b, data[7], S23, 0x676f02d9); /* 31 */
		GG(b, c, d, a, data[12], S24, 0x8d2a4c8a); /* 32 */

		/* Round 3 */
		HH(a, b, c, d, data[5], S31, 0xfffa3942); /* 33 */
		HH(d, a, b, c, data[8], S32, 0x8771f681); /* 34 */
		HH(c, d, a, b, data[11], S33, 0x6d9d6122); /* 35 */
		HH(b, c, d, a, data[14], S34, 0xfde5380c); /* 36 */
		HH(a, b, c, d, data[1], S31, 0xa4beea44); /* 37 */
		HH(d, a, b, c, data[4], S32, 0x4bdecfa9); /* 38 */
		HH(c, d, a, b, data[7], S33, 0xf6bb4b60); /* 39 */
		HH(b, c, d, a, data[10], S34, 0xbebfbc70); /* 40 */
		HH(a, b, c, d, data[13], S31, 0x289b7ec6); /* 41 */
		HH(d, a, b, c, data[0], S32, 0xeaa127fa); /* 42 */
		HH(c, d, a, b, data[3], S33, 0xd4ef3085); /* 43 */
		HH(b, c, d, a, data[6], S34, 0x4881d05); /* 44 */
		HH(a, b, c, d, data[9], S31, 0xd9d4d039); /* 45 */
		HH(d, a, b, c, data[12], S32, 0xe6db99e5); /* 46 */
		HH(c, d, a, b, data[15], S33, 0x1fa27cf8); /* 47 */
		HH(b, c, d, a, data[2], S34, 0xc4ac5665); /* 48 */

		/* Round 4 */
		II(a, b, c, d, data[0], S41, 0xf4292244); /* 49 */
		II(d, a, b, c, data[7], S42, 0x432aff97); /* 50 */
		II(c, d, a, b, data[14], S43, 0xab9423a7); /* 51 */
		II(b, c, d, a, data[5], S44, 0xfc93a039); /* 52 */
		II(a, b, c, d, data[12], S41, 0x655b59c3); /* 53 */
		II(d, a, b, c, data[3], S42, 0x8f0ccc92); /* 54 */
		II(c, d, a, b, data[10], S43, 0xffeff47d); /* 55 */
		II(b, c, d, a, data[1], S44, 0x85845dd1); /* 56 */
		II(a, b, c, d, data[8], S41, 0x6fa87e4f); /* 57 */
		II(d, a, b, c, data[15], S42, 0xfe2ce6e0); /* 58 */
		II(c, d, a, b, data[6], S43, 0xa3014314); /* 59 */
		II(b, c, d, a, data[13], S44, 0x4e0811a1); /* 60 */
		II(a, b, c, d, data[4], S41, 0xf7537e82); /* 61 */
		II(d, a, b, c, data[11], S42, 0xbd3af235); /* 62 */
		II(c, d, a, b, data[2], S43, 0x2ad7d2bb); /* 63 */
		II(b, c, d, a, data[9], S44, 0xeb86d391); /* 64 */

		_state[0] += a;
		_state[1] += b;
		_state[2] += c;
		_state[3] += d;
	}
}