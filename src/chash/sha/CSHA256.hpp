#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CHASH_API CSHA256 : public hash_function {
	private:
		static const uint32_t K[64];

	public:
		CSHA256();
		~CSHA256() { }

	private:
		bool		_init;
		uint32_t	_state[8];
		uint64_t	_count;
		uint8_t		_buffer[64];

	public:
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;

	private:
		void updateFinal();
		void transform(const uint32_t* in256);
		void flush();

	private:
		static constexpr uint32_t rotateRight(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
		static constexpr uint32_t shiftRight(uint32_t a, uint32_t n) { return a >> n; }

		static constexpr uint32_t sigma1(uint32_t x) { return rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22); }
		static constexpr uint32_t sigma2(uint32_t x) { return rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25); }
		static constexpr uint32_t sigma3(uint32_t x) { return rotateRight(x, 7) ^ rotateRight(x, 18) ^ shiftRight(x, 3); }
		static constexpr uint32_t sigma4(uint32_t x) { return rotateRight(x, 17) ^ rotateRight(x, 19) ^ shiftRight(x, 10); }
	};
}