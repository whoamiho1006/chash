#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API crc64 : public hash_function {
	private:
		static constexpr uint64_t POLY_NOMIAL = 0x42f0e1eba9ea3693ull;
		static constexpr uint64_t INIT_VALUE = 0x0000000000000000ull;
		static constexpr uint64_t FINAL_XOR = 0x0000000000000000ull;

	public:
		crc64();
		~crc64() { }

	private:
		bool		_init;
		uint64_t	_table[256];
		uint64_t	_digest;

	public:
		size_t width() const override { return 64; }
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;
	};
}