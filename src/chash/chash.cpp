#include "chash.hpp"

#include "crc/crc16.hpp"
#include "crc/crc32.hpp"
#include "crc/crc64.hpp"

#include "md/md5.hpp"
#include "md/md4.hpp"

#include "sha/sha256.hpp"
#include "sha/sha384.hpp"
#include "sha/sha512.hpp"

#include "ripemd/ripemd128.hpp"
#include "ripemd/ripemd160.hpp"

#include "hmac/hmac_crc.hpp"
#include "hmac/hmac_md.hpp"
#include "hmac/hmac_sha.hpp"
#include "hmac/hmac_ripemd.hpp"

namespace chash {
    CHASH_API std::unique_ptr<hash_function> create(algorithm algorithm) {
		hash_function* instance = nullptr;

        switch (algorithm) {
        case algorithm::CRC16:
			instance = new crc16();
			break;

        case algorithm::CRC32:
			instance = new crc32();
			break;

        case algorithm::CRC64:
			instance = new crc64();
			break;

        case algorithm::SHA256:
			instance = new sha256();
			break;

        case algorithm::SHA384:
			instance = new sha384();
			break;

        case algorithm::SHA512:
			instance = new sha512();
			break;

        case algorithm::MD5:
			instance = new md5();
			break;

        case algorithm::MD4:
			instance = new md4();
			break;

        case algorithm::RipeMD128:
			instance = new ripemd128();
			break;

        case algorithm::RipeMD160:
			instance = new ripemd160();
			break;
        }

		if (!instance) {
			throw invalid_algorithm_error("Unknown algorithm!");
		}

        return std::unique_ptr<hash_function>(instance);
    }

	CHASH_API std::unique_ptr<chash::hmac_function> create_hmac(algorithm algorithm) {
		hmac_function* instance = nullptr;

		switch (algorithm) {
		case algorithm::CRC16:
			instance = new hmac_crc16();
			break;

		case algorithm::CRC32:
			instance = new hmac_crc32();
			break;

		case algorithm::CRC64:
			instance = new hmac_crc64();
			break;

		case algorithm::MD5:
			instance = new hmac_md5();
			break;

		case algorithm::MD4:
			instance = new hmac_md4();
			break;

		case algorithm::SHA256:
			instance = new hmac_sha256();
			break;

		case algorithm::SHA384:
			instance = new hmac_sha384();
			break;

		case algorithm::SHA512:
			instance = new hmac_sha512();
			break;

		case algorithm::RipeMD128:
			instance = new hmac_ripemd128();
			break;

		case algorithm::RipeMD160:
			instance = new hmac_ripemd160();
			break;
		}

		if (!instance) {
			throw invalid_algorithm_error("Unknown algorithm!");
		}

		return std::unique_ptr<hmac_function>(instance);
	}

}