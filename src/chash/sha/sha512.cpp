#include "sha512.hpp"
#include <string.h>

namespace chash {

    const uint64_t sha512::K[128] = {
        0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
        0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
        0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
        0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
        0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
        0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
        0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
        0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
        0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
        0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
        0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
        0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
        0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
        0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
        0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
        0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
        0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
        0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
        0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
        0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
    };

    const uint8_t sha512::PADDING[128] = { 0x80, 0, };

	sha512::sha512()
		: hash_function(algorithm::SHA512), _init(false), _count(0)
	{
	}

	bool sha512::init() {
        if (_init) {
            return false;
        }

        _init = true;
        _state[0] = 0x6A09E667F3BCC908;
        _state[1] = 0xBB67AE8584CAA73B;
        _state[2] = 0x3C6EF372FE94F82B;
        _state[3] = 0xA54FF53A5F1D36F1;
        _state[4] = 0x510E527FADE682D1;
        _state[5] = 0x9B05688C2B3E6C1F;
        _state[6] = 0x1F83D9ABFB41BD6B;
        _state[7] = 0x5BE0CD19137E2179;
        _count = 0;

        ::memset(_buffer, 0, sizeof(_buffer));
        return true;
	}

	void sha512::update(const uint8_t* inBytes, size_t inSize) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
        }

        uint32_t pos = uint32_t(_count) & 0x7f;

        while (inSize) {
            _buffer[pos++] = *inBytes++;
            _count++; inSize--;

            if (pos >= 128) {
                pos = 0;
                flush();
            }
        }
    }

	void sha512::finalize(digest_t& outDigest) {
		if (!_init) {
			throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
		}

		update_final();
		outDigest.reserve(8 * 8);

		for (uint32_t i = 0; i < 8; ++i) {
			outDigest.push_back(uint8_t(_state[i] >> 56));
			outDigest.push_back(uint8_t(_state[i] >> 48));
			outDigest.push_back(uint8_t(_state[i] >> 40));
			outDigest.push_back(uint8_t(_state[i] >> 32));
			outDigest.push_back(uint8_t(_state[i] >> 24));
			outDigest.push_back(uint8_t(_state[i] >> 16));
			outDigest.push_back(uint8_t(_state[i] >> 8));
			outDigest.push_back(uint8_t(_state[i]));
		}

		_init = false;
	}

	void sha512::update_final()
	{
		uint8_t lenBits[8];
		uint64_t length = _count << 3;
		uint32_t index = uint32_t(_count & 0x7f),
			pads = index < 120 ? 120 - index : 128 + 120 - index;

		lenBits[0] = uint8_t(length >> 56);
		lenBits[1] = uint8_t(length >> 48);
		lenBits[2] = uint8_t(length >> 40);
		lenBits[3] = uint8_t(length >> 32);
		lenBits[4] = uint8_t(length >> 24);
		lenBits[5] = uint8_t(length >> 16);
		lenBits[6] = uint8_t(length >> 8);
		lenBits[7] = uint8_t(length);

		update(PADDING, pads);
		update(lenBits, 8);
	}

	void sha512::flush()
    {
        uint64_t block[16];

        for (int32_t i = 0; i < 16; ++i) {
            block[i] =
                (uint64_t(_buffer[i * 8 + 0]) << 56) |
                (uint64_t(_buffer[i * 8 + 1]) << 48) |
                (uint64_t(_buffer[i * 8 + 2]) << 40) |
                (uint64_t(_buffer[i * 8 + 3]) << 32) |
                (uint64_t(_buffer[i * 8 + 4]) << 24) |
                (uint64_t(_buffer[i * 8 + 5]) << 16) |
                (uint64_t(_buffer[i * 8 + 6]) << 8) |
                (uint64_t(_buffer[i * 8 + 7]));
        }

        transform(block);
    }

    void sha512::transform(uint64_t* w)
    {
        uint32_t t;
        uint64_t temp1;
        uint64_t temp2;

        //Initialize the 8 working registers
        uint64_t a = _state[0];
        uint64_t b = _state[1];
        uint64_t c = _state[2];
        uint64_t d = _state[3];
        uint64_t e = _state[4];
        uint64_t f = _state[5];
        uint64_t g = _state[6];
        uint64_t h = _state[7];

#define W(t) w[(t) & 0x0F]
#define CH(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

        //SHA-512 hash computation (alternate method)
        for (t = 0; t < 80; t++)
        {
            //Prepare the message schedule
            if (t >= 16)
                W(t) += sigma4(W(t + 14)) + W(t + 9) + sigma3(W(t + 1));

            //Calculate T1 and T2
            temp1 = h + sigma2(e) + CH(e, f, g) + K[t] + W(t);
            temp2 = sigma1(a) + MAJ(a, b, c);

            //Update the working registers
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        //Update the hash value
        _state[0] += a;
        _state[1] += b;
        _state[2] += c;
        _state[3] += d;
        _state[4] += e;
        _state[5] += f;
        _state[6] += g;
        _state[7] += h;
    }
}