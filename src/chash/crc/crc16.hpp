#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API crc16 : public hash_function {
	private:
		static constexpr uint16_t POLY_NOMIAL = 0x8005;
		static constexpr uint16_t INIT_VALUE = 0x0000;
		static constexpr uint16_t FINAL_XOR = 0x0000;

	public:
		crc16();
		~crc16() { }

	private:
		bool		_init;
		uint16_t	_table[256];
		uint16_t	_digest;

	public:
		size_t width() const override { return 16; }
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;
	};
}