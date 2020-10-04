#pragma once
#include "Macros.hpp"
#include <string.h>
#include <vector>

namespace chash {
	enum class EAlgorithm {
		Unknown		= 0x0000,
		CRC16		= 0x1000,		// --> IBM Poly-Nomial.
		CRC32		= 0x1001,		// --> IEEE 802.3
		CRC64		= 0x1002,		// --> ISO Poly-Nomial.
		SHA256		= 0x2000,
		SHA384		= 0x2001,
		SHA512		= 0x2002,
		MD5			= 0x3000,
		MD4			= 0x3001,
		RipeMD128	= 0x4000,
		RipeMD160	= 0x4001,
	};

	enum class EAlgorithmErrno {
		Succeed = 0,
		InvalidAlgorithm,
		InvalidState,
		//InvalidDigest
	};

	typedef std::vector<uint8_t> CDigest;

	class IAlgorithm {
	public:
		IAlgorithm(EAlgorithm type)
			: _type(type), _errno(EAlgorithmErrno::Succeed)
		{ 
		}

		virtual ~IAlgorithm() { }

	private:
		EAlgorithm _type;
		EAlgorithmErrno _errno;

	protected:
		inline void setType(EAlgorithm _type) {
			this->_type = _type;
		}

		inline void setError(EAlgorithmErrno _errno) {
			this->_errno = _errno;
		}

	public:
		/* get algorithm type. */
		inline EAlgorithm type() const { return _type; }

		/* get algorithm state. */
		inline EAlgorithmErrno error() const { return _errno; }

		/* initiate the algorithm. */
		virtual bool init() = 0;

		/* update the algorithm state by given bytes. */
		virtual bool update(const uint8_t* inBytes, size_t inSize) = 0;

		/* finalize the algorithm and digest. */
		virtual bool finalize(CDigest& outDigest) = 0;
		
		/* compute hash with digest. */
		virtual EAlgorithmErrno compute(CDigest& outDigest, const uint8_t* inBytes, size_t inSize) {
			if (init()) {
				update(inBytes, inSize);
				finalize(outDigest);
			}

			return error();
		}
	};

	/* Digest to hex. */
	inline std::string toHex(const CDigest& inDigest) {
		std::string outHex;

		outHex.reserve(inDigest.size() << 1);
		for(uint8_t b : inDigest) {
			int32_t fr = b / 16;
			int32_t bk = b % 16;

			if (fr < 10) outHex.push_back('0' + fr);
			else outHex.push_back('a' + (fr - 10));

			if (bk < 10) outHex.push_back('0' + bk);
			else outHex.push_back('a' + (bk - 10));
		}

		return outHex;
	}

	/* Comparator between two CDigests. */
	inline bool operator ==(const CDigest& left, const CDigest& right) {
		if (left.size() == right.size()) {
			return !::memcmp(&left[0], &right[0], left.size());
		}

		return false;
	}


	/* Comparator between two CDigests. */
	inline bool operator !=(const CDigest& left, const CDigest& right) {
		if (left.size() == right.size()) {
			return ::memcmp(&left[0], &right[0], left.size());
		}

		return true;
	}
}