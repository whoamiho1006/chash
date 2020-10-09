#pragma once
#include "chash/boilerplate.hpp"

namespace chash {
	enum class algorithm {
		Unknown		= 0x0000,
		CRC16		= 0x1000,		// --> IBM Poly-Nomial.
		CRC32		= 0x1001,		// --> IEEE 802.3
		CRC64		= 0x1002,		// --> ECMA 182
		SHA256		= 0x2000,
		SHA384		= 0x2001,
		SHA512		= 0x2002,
		MD5			= 0x3000,
		MD4			= 0x3001,
		RipeMD128	= 0x4000,
		RipeMD160	= 0x4001,
	};

	typedef std::vector<uint8_t> digest_t;

	/* Invalid Algorithm error. */
	class invalid_algorithm_error : public std::runtime_error {
	public:
		invalid_algorithm_error(const char* _what) 
			: std::runtime_error(_what)
		{
		}
	};

	/* Invalid State error. */
	class invalid_state_error : public std::runtime_error {
	public:
		invalid_state_error(const char* _what)
			: std::runtime_error(_what)
		{
		}
	};

	/* hash_function interface */
	class hash_function {
	public:
		hash_function(algorithm type)
			: _type(type)
		{ 
		}

		virtual ~hash_function() { }

	private:
		algorithm _type;

	public:
		/* get algorithm type. */
		inline algorithm type() const { return _type; }

		/* bit width of this algorithm. */
		virtual size_t width() const = 0;

		/* initiate the algorithm. */
		virtual bool init() = 0;

		/* update the algorithm state by given bytes. */
		virtual void update(const uint8_t* inBytes, size_t inSize) = 0;

		/* finalize the algorithm and digest. */
		virtual void finalize(digest_t& outDigest) = 0;
		
		/* compute hash with digest. */
		virtual bool compute(digest_t& outDigest, const uint8_t* inBytes, size_t inSize) {
			if (init()) {
				update(inBytes, inSize);
				finalize(outDigest);
				return true;
			}

			return false;
		}
	};

	/* Digest to hex. */
	inline std::string to_hex(const digest_t& inDigest) {
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