#include "chash.hpp"

#include "crc/CCRC16.hpp"
#include "crc/CCRC32.hpp"
#include "crc/CCRC64.hpp"

#include "md/CMD5.hpp"
#include "md/CMD4.hpp"

#include "sha/CSHA256.hpp"
#include "sha/CSHA384.hpp"
#include "sha/CSHA512.hpp"

#include "ripemd/CRipeMD128.hpp"
#include "ripemd/CRipeMD160.hpp"

namespace chash {
    CHASH_API std::unique_ptr<hash_function> create(algorithm algorithm) {
		hash_function* instance = nullptr;

        switch (algorithm) {
        case algorithm::CRC16:
			instance = new CCRC16();
			break;

        case algorithm::CRC32:
			instance = new CCRC32();
			break;

        case algorithm::CRC64:
			instance = new CCRC64();
			break;

        case algorithm::SHA256:
			instance = new CSHA256();
			break;

        case algorithm::SHA384:
			instance = new CSHA384();
			break;

        case algorithm::SHA512:
			instance = new CSHA512();
			break;

        case algorithm::MD5:
			instance = new CMD5();
			break;

        case algorithm::MD4:
			instance = new CMD4();
			break;

        case algorithm::RipeMD128:
			instance = new CRipeMD128();
			break;

        case algorithm::RipeMD160:
			instance = new CRipeMD160();
			break;
        }

		if (!instance) {
			throw invalid_algorithm_error("Unknown algorithm!");
		}

        return std::unique_ptr<hash_function>(instance);
    }
}