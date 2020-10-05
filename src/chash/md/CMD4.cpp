#include "CMD4.hpp"

#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s) a += F(b, c, d) + (x), a = ROTATE_LEFT(a, s)
#define GG(a, b, c, d, x, s) a += G(b, c, d) + (x) + 0x5A827999, a = ROTATE_LEFT(a, s)
#define HH(a, b, c, d, x, s) a += H(b, c, d) + (x) + 0x6ED9EBA1, a = ROTATE_LEFT(a, s)

namespace chash {
	const uint8_t CMD4::PADDING[64] = { 0x80, 0, };

	CMD4::CMD4()
		: hash_function(algorithm::MD4), _init(false), _count(0)
	{
	}

	bool CMD4::init() {
		if (_init) {
			return false;
		}

		_init = true;

		_state[0] = 0x67452301;
		_state[1] = 0xEFCDAB89;
		_state[2] = 0x98BADCFE;
		_state[3] = 0x10325476;

		_count = 0;
		return true;
	}

	void CMD4::update(const uint8_t* inBytes, size_t inSize) {
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

	void CMD4::finalize(digest_t& outDigest) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
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

	void CMD4::updateFinal() {
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

	void CMD4::flush() {
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

	void CMD4::transform(const uint32_t* data) {
		uint32_t a = _state[0];
		uint32_t b = _state[1];
		uint32_t c = _state[2];
		uint32_t d = _state[3];

		//Round 1
		FF(a, b, c, d, data[0], 3);
		FF(d, a, b, c, data[1], 7);
		FF(c, d, a, b, data[2], 11);
		FF(b, c, d, a, data[3], 19);
		FF(a, b, c, d, data[4], 3);
		FF(d, a, b, c, data[5], 7);
		FF(c, d, a, b, data[6], 11);
		FF(b, c, d, a, data[7], 19);
		FF(a, b, c, d, data[8], 3);
		FF(d, a, b, c, data[9], 7);
		FF(c, d, a, b, data[10], 11);
		FF(b, c, d, a, data[11], 19);
		FF(a, b, c, d, data[12], 3);
		FF(d, a, b, c, data[13], 7);
		FF(c, d, a, b, data[14], 11);
		FF(b, c, d, a, data[15], 19);

		//Round 2
		GG(a, b, c, d, data[0], 3);
		GG(d, a, b, c, data[4], 5);
		GG(c, d, a, b, data[8], 9);
		GG(b, c, d, a, data[12], 13);
		GG(a, b, c, d, data[1], 3);
		GG(d, a, b, c, data[5], 5);
		GG(c, d, a, b, data[9], 9);
		GG(b, c, d, a, data[13], 13);
		GG(a, b, c, d, data[2], 3);
		GG(d, a, b, c, data[6], 5);
		GG(c, d, a, b, data[10], 9);
		GG(b, c, d, a, data[14], 13);
		GG(a, b, c, d, data[3], 3);
		GG(d, a, b, c, data[7], 5);
		GG(c, d, a, b, data[11], 9);
		GG(b, c, d, a, data[15], 13);

		//Round 3
		HH(a, b, c, d, data[0], 3);
		HH(d, a, b, c, data[8], 9);
		HH(c, d, a, b, data[4], 11);
		HH(b, c, d, a, data[12], 15);
		HH(a, b, c, d, data[2], 3);
		HH(d, a, b, c, data[10], 9);
		HH(c, d, a, b, data[6], 11);
		HH(b, c, d, a, data[14], 15);
		HH(a, b, c, d, data[1], 3);
		HH(d, a, b, c, data[9], 9);
		HH(c, d, a, b, data[5], 11);
		HH(b, c, d, a, data[13], 15);
		HH(a, b, c, d, data[3], 3);
		HH(d, a, b, c, data[11], 9);
		HH(c, d, a, b, data[7], 11);
		HH(b, c, d, a, data[15], 15);

		//Update the hash value
		_state[0] += a;
		_state[1] += b;
		_state[2] += c;
		_state[3] += d;
	}
}