#pragma once
#include "chash/IAlgorithm.hpp"

#if defined(_WIN32) || defined(_WIN64)
#ifdef __CHASH_EXPORTS__
#define CHASH_API __declspec(dllexport)
#else
#define CHASH_API __declspec(dllimport)
#endif
#else
#define CHASH_API
#endif

namespace chash {
	CHASH_API IAlgorithm* createAlgorithm(EAlgorithm algorithm);

	template<EAlgorithm Algo>
	class TAlgorithm : public IAlgorithm {
	public:
		TAlgorithm() 
			: IAlgorithm(EAlgorithm::Unknown)
		{
			algorithm = createAlgorithm(Algo);
			setType(Algo);
		}

		TAlgorithm(const TAlgorithm<Algo>&) = delete;
		TAlgorithm(TAlgorithm<Algo>&& other) 
			: algorithm(other.algorithm)
		{
		}

		~TAlgorithm() {
			if (algorithm) {
				delete algorithm;
			}
		}

	private:
		IAlgorithm* algorithm;

	public:
		inline operator bool() const {
			return algorithm;
		}

		inline bool operator !() const {
			return !algorithm;
		}

		inline TAlgorithm<Algo>& operator =(const TAlgorithm<Algo>&) = delete;
		inline TAlgorithm<Algo>& operator =(TAlgorithm<Algo>&& other) {
			if (this != &other) 
				std::swap(algorithm, other.algorithm);

			return *this;
		}
		
	public:
		/* initiate the algorithm. */
		virtual bool init() override {
			if (algorithm) {
				bool retVal = algorithm->init();

				setError(algorithm->error());
				return retVal;
			}

			setError(EAlgorithmErrno::InvalidAlgorithm);
			return false;
		}

		/* update the algorithm state by given bytes. */
		virtual bool update(const uint8_t* inBytes, size_t inSize) override {
			if (algorithm) {
				bool retVal = algorithm->update(inBytes, inSize);

				setError(algorithm->error());
				return retVal;
			}

			setError(EAlgorithmErrno::InvalidAlgorithm);
			return false;
		}

		/* finalize the algorithm and digest. */
		virtual bool finalize(CDigest& outDigest) override {
			if (algorithm) {
				bool retVal = algorithm->finalize(outDigest);

				setError(algorithm->error());
				return retVal;
			}

			setError(EAlgorithmErrno::InvalidAlgorithm);
			return false;
		}
	};
}