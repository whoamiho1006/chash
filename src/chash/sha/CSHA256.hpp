#pragma once
#include "chash/IDigest.hpp"
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
		virtual IDigest* create() const override {
			return new TDigest<32>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;

	private:
		void transform(const uint32_t* in256);
		void flush();
	};
}