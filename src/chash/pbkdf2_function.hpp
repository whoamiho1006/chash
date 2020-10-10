#pragma once
#include "chash/boilerplate.hpp"
#include "chash/hmac_function.hpp"
#include <memory>

namespace chash {
	typedef std::unique_ptr<hmac_function> hmac_ptr;

	/* pbkdf2 password generator. */
	class pbkdf2_function {
	public:
		pbkdf2_function(hmac_ptr&& hmac) : hmac(std::forward<hmac_ptr>(hmac)) { }

	private:
		hmac_ptr hmac;

	private:
		static constexpr size_t minVal(size_t x, size_t y) { return x > y ? y : x; }

	public:
		inline void gen(digest_t& out_digest,
			const uint8_t* passwd, size_t passwd_length,
			const uint8_t* salt, size_t salt_length,
			const size_t iteration, const size_t key_length)
		{
			digest_t pass1, pass2, last;

			size_t counter = 1;
			size_t gen_key = 0;

			while (gen_key < key_length) {
				uint8_t c[4];

				c[0] = uint8_t(counter >> 24);
				c[1] = uint8_t(counter >> 16);
				c[2] = uint8_t(counter >> 8);
				c[3] = uint8_t(counter);

				if (!hmac->init(passwd, passwd_length)) {
					throw invalid_state_error("HMAC algorithm couldn't be initiated.");
				}

				hmac->update(salt, salt_length);
				hmac->update(c, sizeof(c));

				pass1.clear();
				hmac->finalize(pass1);

				last = pass1;

				for (size_t i = 1; i < iteration; ++i) {
					if (!hmac->init(passwd, passwd_length)) {
						throw invalid_state_error("HMAC algorithm couldn't be initiated.");
					}

					hmac->update(&last[0], last.size());
					hmac->finalize(pass2);

					for (size_t j = 0; j < pass2.size(); j++) {
						pass1[j] ^= pass2[j];
					}

					last = std::move(pass2);
				}

				size_t key_bytes = minVal(key_length - gen_key, pass1.size());
				for (int32_t i = 0; i < key_bytes; ++i) {
					out_digest.push_back(pass1[i]);
				}

				gen_key += key_bytes;
				++counter;
			}
		}
	};
}