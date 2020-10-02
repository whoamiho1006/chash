#pragma once
#include "chash/IDigest.hpp"
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CCRC32 : public IAlgorithm {
	private:
		static const uint32_t POLY_NOMIAL = 0x04c11db7u;

	public:
		CCRC32();
		~CCRC32() { }

	private:
		bool		_init;
		uint32_t	_table[256];
		uint32_t	_digest;

	public:
		virtual IDigest* create() const override {
			return new TDigest<sizeof(uint32_t)>();
		}

		bool init() override;
		bool update(const uint8_t* inBytes, size_t inSize) override;
		bool finalize(IDigest* outDigest) override;
	};
}