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

	typedef std::vector<uint8_t> CDigest;

	/* Invalid Algorithm error. */
	class CInvalidAlgorithmError : public std::exception {
	public:
		CInvalidAlgorithmError(...) 
			: std::exception()
		{
		}
	};

	/* Invalid State error. */
	class CInvalidStateError : public std::exception {
	public:
		CInvalidStateError(...)
			: std::exception()
		{
		}
	};

	/* IAlgorithm interface */
	class IAlgorithm {
	public:
		IAlgorithm(EAlgorithm type)
			: _type(type)
		{ 
		}

		virtual ~IAlgorithm() { }

	private:
		EAlgorithm _type;

	protected:
		inline void setType(EAlgorithm _type) {
			this->_type = _type;
		}

	public:
		/* get algorithm type. */
		inline EAlgorithm type() const { return _type; }

		/* initiate the algorithm. */
		virtual bool init() = 0;

		/* update the algorithm state by given bytes. */
		virtual void update(const uint8_t* inBytes, size_t inSize) = 0;

		/* finalize the algorithm and digest. */
		virtual void finalize(CDigest& outDigest) = 0;
		
		/* compute hash with digest. */
		virtual bool compute(CDigest& outDigest, const uint8_t* inBytes, size_t inSize) {
			if (init()) {
				update(inBytes, inSize);
				finalize(outDigest);
				return true;
			}

			return false;
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
}