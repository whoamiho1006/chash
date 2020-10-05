#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CHASH_API CSHA512 : public IAlgorithm {
	private:
		static const uint64_t K[128];
		static const uint8_t PADDING[128];

	public:
		CSHA512();
		~CSHA512() { }

	private:
		bool		_init;
		uint64_t	_state[8];
		uint64_t	_count;
		uint8_t		_buffer[128];

	public:
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(CDigest& outDigest) override;

	private:
		void updateFinal();
		void flush();
		void transform(uint64_t* in256);

	private:
		static constexpr uint64_t rotateRight(uint64_t x, uint32_t n) { return (x >> n) | (x << (64 - n)); }
		static constexpr uint64_t shiftRight(uint64_t a, uint32_t n) { return a >> n; }

		static constexpr uint64_t sigma1(uint64_t x) { return rotateRight(x, 28) ^ rotateRight(x, 34) ^ rotateRight(x, 39); }
		static constexpr uint64_t sigma2(uint64_t x) { return rotateRight(x, 14) ^ rotateRight(x, 18) ^ rotateRight(x, 41); }
		static constexpr uint64_t sigma3(uint64_t x) { return rotateRight(x, 1) ^ rotateRight(x, 8) ^ shiftRight(x, 7); }
		static constexpr uint64_t sigma4(uint64_t x) { return rotateRight(x, 19) ^ rotateRight(x, 61) ^ shiftRight(x, 6); }
	};
}