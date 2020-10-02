#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CCRC64 : public IAlgorithm {
	private:
		static const uint32_t POLY_NOMIAL = 0x000000000000001bull;
		static constexpr uint32_t INIT_VALUE = 0x0000000000000000ull;

	public:
		CCRC64();
		~CCRC64() { }

	private:
		bool		_init;
		uint64_t	_table[256];
		uint64_t	_digest;

	public:
		virtual IDigest* create() const override {
			return new TDigest<sizeof(uint64_t)>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;
	};
}