#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API sha512 : public hash_function {
	private:
		static const uint64_t K[128];
		static const uint8_t PADDING[128];

	public:
		sha512();
		~sha512() { }

	private:
		bool		_init;
		uint64_t	_state[8];
		uint64_t	_count;
		uint8_t		_buffer[128];

	public:
		size_t width() const override { return 512; }
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;

	private:
		void update_final();
		void flush();
		void transform(uint64_t* in256);

	private:
		static constexpr uint64_t rotate_right(uint64_t x, uint32_t n) { return (x >> n) | (x << (64 - n)); }
		static constexpr uint64_t shift_right(uint64_t a, uint32_t n) { return a >> n; }

		static constexpr uint64_t sigma1(uint64_t x) { return rotate_right(x, 28) ^ rotate_right(x, 34) ^ rotate_right(x, 39); }
		static constexpr uint64_t sigma2(uint64_t x) { return rotate_right(x, 14) ^ rotate_right(x, 18) ^ rotate_right(x, 41); }
		static constexpr uint64_t sigma3(uint64_t x) { return rotate_right(x, 1) ^ rotate_right(x, 8) ^ shift_right(x, 7); }
		static constexpr uint64_t sigma4(uint64_t x) { return rotate_right(x, 19) ^ rotate_right(x, 61) ^ shift_right(x, 6); }
	};
}