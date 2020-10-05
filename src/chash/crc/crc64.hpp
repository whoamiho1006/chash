#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API crc64 : public hash_function {
	private:
		static const uint32_t POLY_NOMIAL = 0x000000000000001bull;
		static constexpr uint32_t INIT_VALUE = 0x0000000000000000ull;
		static constexpr uint32_t FINAL_XOR = 0x0000000000000000ull;

	public:
		crc64();
		~crc64() { }

	private:
		bool		_init;
		uint64_t	_table[256];
		uint64_t	_digest;

	public:
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;
	};
}