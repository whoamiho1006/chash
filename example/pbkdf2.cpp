#include "chash/chash.hpp"
#include "chash/hmac_function.hpp"

using namespace chash;

int pbkdf2_hmac_sha256()
{
	if (auto hmac = create_hmac(algorithm::SHA256)) {
		pbkdf2_function pbkdf2(std::move(hmac));
		digest_t gen_key;

		pbkdf2.gen(gen_key, 
			(uint8_t*)"1234", 4, 
			(uint8_t*)"salt", 4,
			4, 16);

		printf("PBKDF2 HMAC SHA256(1234, salt, 16): %s\n", to_hex(gen_key).c_str());
	}

	return -1;
}