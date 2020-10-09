#pragma once
#include "chash/hash_function.hpp"

namespace chash {

	/*	hmac_function interface. 
		almost same with hash_function interface, 
		but, init method is different. */
	class hmac_function {
	public:
		hmac_function(algorithm type) : _type(type) { }
		virtual ~hmac_function() { }

	private:
		algorithm _type;

	public:
		/* get algorithm type. */
		inline algorithm type() const { return _type; }

		/* initiate the algorithm. */
		virtual bool init(const uint8_t* key, size_t size) = 0;

		/* update the algorithm state by given bytes. */
		virtual void update(const uint8_t* in_bytes, size_t size) = 0;

		/* finalize the algorithm and digest. */
		virtual void finalize(digest_t& outDigest) = 0;

		/* compute hash with digest. */
		inline bool compute(digest_t& outDigest, 
			const uint8_t* key, size_t keySize,
			const uint8_t* inBytes, size_t inSize) 
		{
			if (init(key, keySize)) {
				update(inBytes, inSize);
				finalize(outDigest);
				return true;
			}

			return false;
		}
	};
}