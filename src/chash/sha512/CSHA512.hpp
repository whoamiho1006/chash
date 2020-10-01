#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CSHA512 : public IAlgorithm {
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
		virtual IDigest* create() const override {
			return new TDigest<64>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;

	private:
		void flush();
		void transform(uint64_t* in256);
	};
}