#pragma once
#include "chash/IAlgorithm.hpp"

namespace chash {
	class CHASH_API CRipeMD160 : public hash_function {
	private:
		static const uint8_t PADDING[64];

	public:
		CRipeMD160();
		~CRipeMD160() { }

	private:
		bool		_init;
		uint64_t	_count;
		uint32_t	_state[5];
		uint8_t		_buffer[64];

	public:
		bool init() override;
		void update(const uint8_t* inBytes, size_t inSize) override;
		void finalize(digest_t& outDigest) override;

	private:
		void updateFinal();
		void flush();
		void transform(const uint32_t* data);
	};
}