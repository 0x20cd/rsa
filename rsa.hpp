#ifndef RSA_HPP
#define RSA_HPP
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/independent_bits.hpp>
#include <iostream>

namespace rsa2048
{
	using uint2048_t = boost::multiprecision::number <
		boost::multiprecision::cpp_int_backend < 2048, 2048,
			boost::multiprecision::unsigned_magnitude,
			boost::multiprecision::unchecked, void> >;
	using uint4096_t = boost::multiprecision::number <
		boost::multiprecision::cpp_int_backend < 4096, 4096,
			boost::multiprecision::unsigned_magnitude,
			boost::multiprecision::unchecked, void> >;

	extern const uint2048_t E;

	uint2048_t& pow(const uint2048_t &x, const uint2048_t &y, const uint2048_t &z, uint2048_t &r);

	uint2048_t& enc(const uint2048_t &pt, uint2048_t &r, const uint2048_t &pub);
	uint2048_t& dec(const uint2048_t &ct, uint2048_t &r, const uint2048_t &pub, const uint2048_t &priv);

	// randA, randB - used for generating the key and for primality testing respectively
	template <class RandomA, class RandomB>
	void newKey(RandomA &randA, RandomB &randB, uint2048_t &pub, uint2048_t &priv)
	{
		using namespace boost::multiprecision;
		using namespace boost::random;

		static independent_bits_engine<RandomA, 1023, uint1024_t> gen(randA);

		uint2048_t phi = 4, phi_phi = 2;
		pub = 1;

		for (int i = 2; i;)
		{
			uint1024_t p = gen();
			bit_set(p, 1023);
			p -= p % 12 + 1; // p = 12k - 1
			uint1024_t p0 = p >> 1;

			if (!miller_rabin_test(p, 25, randB) ||
				!miller_rabin_test(p0, 25, randB)) continue;

			pub *= p;
			phi *= p0;
			phi_phi *= (p0-1);

			std::cout << "\n\n" << p;
			std::cout.flush();

			--i;
		}

		pow(E, --phi_phi, phi, priv);
	}
}

#endif//RSA_HPP
