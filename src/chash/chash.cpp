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
    CHASH_API IAlgorithm* createAlgorithm(EAlgorithm algorithm) {
        switch (algorithm) {
        case EAlgorithm::CRC16:
            return new CCRC16();

        case EAlgorithm::CRC32:
            return new CCRC32();

        case EAlgorithm::CRC64:
            return new CCRC64();

        case EAlgorithm::SHA256:
            return new CSHA256();

        case EAlgorithm::SHA384:
            return new CSHA384();

        case EAlgorithm::SHA512:
            return new CSHA512();

        case EAlgorithm::MD5:
            return new CMD5();

        case EAlgorithm::MD4:
            return new CMD4();

        case EAlgorithm::RipeMD128:
            return new CRipeMD128();

        case EAlgorithm::RipeMD160:
            return new CRipeMD160();
        }

        return nullptr;
    }
}