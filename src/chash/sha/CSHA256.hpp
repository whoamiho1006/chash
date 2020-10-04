#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CSHA256 : public IAlgorithm {
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
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(CDigest& outDigest) override;

	private:
		void updateFinal();
		void transform(const uint32_t* in256);
		void flush();
	};
}