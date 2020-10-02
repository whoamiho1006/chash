#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CCRC16 : public IAlgorithm {
	private:
		static constexpr uint16_t POLY_NOMIAL = 0x8005;
		static constexpr uint16_t INIT_VALUE = 0x0000;
		static const uint16_t TABLE[256];

	public:
		CCRC16();
		~CCRC16() { }

	private:
		bool		_init;
		uint16_t	_table[256];
		uint16_t	_digest;

	public:
		virtual IDigest* create() const override {
			return new TDigest<sizeof(uint16_t)>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;
	};
}