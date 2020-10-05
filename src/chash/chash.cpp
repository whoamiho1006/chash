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
    CHASH_API std::unique_ptr<IAlgorithm> createAlgorithm(EAlgorithm algorithm) {
		IAlgorithm* instance = nullptr;

        switch (algorithm) {
        case EAlgorithm::CRC16:
			instance = new CCRC16();
			break;

        case EAlgorithm::CRC32:
			instance = new CCRC32();
			break;

        case EAlgorithm::CRC64:
			instance = new CCRC64();
			break;

        case EAlgorithm::SHA256:
			instance = new CSHA256();
			break;

        case EAlgorithm::SHA384:
			instance = new CSHA384();
			break;

        case EAlgorithm::SHA512:
			instance = new CSHA512();
			break;

        case EAlgorithm::MD5:
			instance = new CMD5();
			break;

        case EAlgorithm::MD4:
			instance = new CMD4();
			break;

        case EAlgorithm::RipeMD128:
			instance = new CRipeMD128();
			break;

        case EAlgorithm::RipeMD160:
			instance = new CRipeMD160();
			break;
        }

		if (!instance) {
			throw CInvalidAlgorithmError("Unknown algorithm!");
		}

        return std::unique_ptr<IAlgorithm>(instance);
    }
}