#pragma once
#include "Macros.hpp"

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
		InvalidState,
		InvalidDigest
	};

	class IDigest;
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
		inline void setError(EAlgorithmErrno _errno) {
			this->_errno = _errno;
		}

	public:
		/* get algorithm type. */
		inline EAlgorithm type() const { return _type; }

		/* get algorithm state. */
		inline EAlgorithmErrno error() const { return _errno; }

		/* create a new digest. */
		virtual IDigest* create() const = 0;

		/* initiate the algorithm. */
		virtual bool init() = 0;

		/* update the algorithm state by given bytes. */
		virtual bool update(const uint8_t* inBytes, size_t inSize) = 0;

		/* finalize the algorithm. */
		virtual bool finalize(IDigest* outDigest) = 0;

		/* compute hash with digest. */
		virtual EAlgorithmErrno compute(IDigest* outDigest, const uint8_t* inBytes, size_t inSize) {
			if (init()) {
				update(inBytes, inSize);
				finalize(outDigest);
				return error();
			}

			return error();
		}
	};

}