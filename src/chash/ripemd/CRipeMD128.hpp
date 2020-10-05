#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CHASH_API CRipeMD128 : public IAlgorithm {
	private:
		static const uint8_t PADDING[64];

	public:
		CRipeMD128();
		~CRipeMD128() { }

	private:
		bool		_init;
		uint64_t	_count;
		uint32_t	_state[4];
		uint8_t		_buffer[64];

	public:
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(CDigest& outDigest) override;

	private:
		void updateFinal();
		void flush();
		void transform(const uint32_t* data);
	};
}