#pragma once
#include "Macros.hpp"

namespace chash {
	enum class EAlgorithm {
		Unknown = 0,
		SHA256,
		MD5,
		RipeMD160
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