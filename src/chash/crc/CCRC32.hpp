#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CCRC32 : public IAlgorithm {
	private:
		static const uint32_t POLY_NOMIAL = 0x4C11DB7;
		static constexpr uint32_t INIT_VALUE = 0x00000000;
		static constexpr uint32_t FINAL_XOR = 0x00000000;

	public:
		CCRC32();
		~CCRC32() { }

	private:
		bool		_init;
		uint32_t	_table[256];
		uint32_t	_digest;

	public:
		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(CDigest& outDigest) override;
	};
}