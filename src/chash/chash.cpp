#include "chash.hpp"
#include "crc/CCRC16.hpp"
#include "crc/CCRC32.hpp"
#include "md5/CMD5.hpp"
#include "sha256/CSHA256.hpp"
#include "sha384/CSHA384.hpp"
#include "sha512/CSHA512.hpp"
#include "ripemd160/CRipeMD160.hpp"

namespace chash {
    CHASH_API IAlgorithm* createAlgorithm(EAlgorithm algorithm) {
        switch (algorithm) {
        case EAlgorithm::CRC16:
            return new CCRC16();

        case EAlgorithm::CRC32:
            return new CCRC32();

        case EAlgorithm::SHA256:
            return new CSHA256();

        case EAlgorithm::SHA384:
            return new CSHA384();

        case EAlgorithm::SHA512:
            return new CSHA512();

        case EAlgorithm::MD5:
            return new CMD5();

        case EAlgorithm::RipeMD160:
            return new CRipeMD160();
        }

        return nullptr;
    }
}