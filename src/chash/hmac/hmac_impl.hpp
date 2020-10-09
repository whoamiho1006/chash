#pragma once
#include "chash/hmac_function.hpp"

namespace chash {

	template<typename hash_type, size_t __block_size>
	class hmac_impl : public hmac_function {
	private:
		static constexpr uint8_t I_PAD = 0x36;
		static constexpr uint8_t O_PAD = 0x5C;
		static constexpr size_t BLOCK_SIZE = __block_size;

	public:
		hmac_impl()
			: hmac_function(_algo.type()), _init(false)
		{
		}

		~hmac_impl() { }

	private:
		bool _init;
		hash_type _algo;

		uint8_t _i_key[BLOCK_SIZE];
		uint8_t _o_key[BLOCK_SIZE];

	public:
		bool init(const uint8_t* key, size_t size) {
			if (_init) {
				return false;
			}

			_init = true;
			if (size > BLOCK_SIZE) {
				digest_t s_key;

				_algo.compute(s_key, key, size);
				return __init(&s_key[0], s_key.size());
			}

			return __init(key, size);
		}

	private:
		inline bool __init(const uint8_t* key, size_t size) {
			for (size_t i = 0; i < size; ++i) {
				_o_key[i] = O_PAD ^ key[i];
				_i_key[i] = I_PAD ^ key[i];
			}

			for (size_t i = size; i < BLOCK_SIZE; ++i) {
				_o_key[i] = O_PAD ^ 0;
				_i_key[i] = I_PAD ^ 0;
			}

			_algo.init();
			_algo.update(_i_key, BLOCK_SIZE);
			return true;
		}

	public:
		void update(const uint8_t* in_bytes, size_t size) {
			if (!_init) {
				throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
			}

			_algo.update(in_bytes, size);
		}

		void finalize(digest_t& outDigest) {
			if (!_init) {
				throw invalid_state_error("Can't perform anything for non-initiated algorithm!");
			}

			digest_t digest;
			_algo.finalize(digest);
			_algo.init();

			_algo.update(_o_key, BLOCK_SIZE);
			_algo.update(&digest[0], digest.size());

			_algo.finalize(outDigest);
		}
	};
}