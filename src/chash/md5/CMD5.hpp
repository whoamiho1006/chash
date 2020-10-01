#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CMD5 : public IAlgorithm {
	private:
		static const uint8_t PADDING[64];

	public:
		CMD5();
		~CMD5() { }

	private:
		bool		_init;
		uint32_t	_state[4];
		uint64_t	_count;
		uint8_t		_buffer[64];

	public:
		virtual IDigest* create() const override {
			return new TDigest<16>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;

	private:
		void flush();
		void transform(const uint32_t* data);
	};
}