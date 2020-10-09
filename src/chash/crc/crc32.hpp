#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API crc32 : public hash_function {
	private:
		static constexpr uint32_t POLY_NOMIAL = 0x4C11DB7;
		static constexpr uint32_t INIT_VALUE = 0xFFFFFFFF;
		static constexpr uint32_t FINAL_XOR = 0xFFFFFFFF;

	public:
		crc32();
		~crc32() { }

	private:
		bool		_init;
		uint32_t	_table[256];
		uint32_t	_digest;

	public:
		size_t width() const override { return 32; }
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;
	};
}