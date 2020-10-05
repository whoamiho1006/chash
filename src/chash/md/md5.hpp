#pragma once
#include "chash/hash_function.hpp"

namespace chash {
	class CHASH_API md5 : public hash_function {
	private:
		static const uint8_t PADDING[64];

	public:
		md5();
		~md5() { }

	private:
		bool		_init;
		uint32_t	_state[4];
		uint64_t	_count;
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