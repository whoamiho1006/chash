#pragma once
#include "Macros.hpp"
#include <string>

namespace chash {
	class IDigest {
	public:
		virtual ~IDigest() { }

	public:
		/* get bytes pointer. */
		virtual uint8_t* bytes() const = 0;

		/* get size in bytes. */
		virtual size_t size() const = 0;

	public:
		inline std::string toHex() {
			std::string outHex;

			uint8_t* bytes = this->bytes();
			size_t size = this->size();

			outHex.reserve(size << 1);
			for (size_t i = 0; i < size; ++i) {
				int32_t b = bytes[i];

				int32_t fr = b / 16;
				int32_t bk = b % 16;

				if (fr < 10) outHex.push_back('0' + fr);
				else outHex.push_back('a' + (fr - 10));

				if (bk < 10) outHex.push_back('0' + bk);
				else outHex.push_back('a' + (bk - 10));
			}

			return outHex;
		}
	};

	/* Digest in template. */
	template<size_t Size>
	class TDigest : public IDigest {
	public:
		TDigest() {
			for (size_t i = 0; i < Size; ++i)
				_bytes[i] = 0;
		}

	private:
		mutable uint8_t _bytes[Size];

	public:
		/* get bytes pointer. */
		virtual uint8_t* bytes() const { return _bytes; }

		/* get size in bytes. */
		virtual size_t size() const { return Size;  }
	};
}