#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CSHA384 : public IAlgorithm {
	private:
		static const uint64_t K[128];
		static const uint8_t PADDING[128];

	public:
		CSHA384();
		~CSHA384() { }

	private:
		bool		_init;
		uint64_t	_state[8];
		uint64_t	_count;
		uint8_t		_buffer[128];

	public:
		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(CDigest& outDigest) override;

	private:
		void updateFinal();
		void flush();
		void transform(uint64_t* in256);
	};
}