#include "rsa.hpp"
using rsa2048::uint2048_t;
using rsa2048::uint4096_t;

const uint2048_t rsa2048::E = 0x10001;

uint2048_t& rsa2048::pow(const uint2048_t &x, const uint2048_t &y, const uint2048_t &z, uint2048_t &r)
{
	uint4096_t sq, t, res;

	for (sq = x, t = y, res = 1; t; t >>= 1, sq *= sq, sq %= z)
		if (t&1)
			res *= sq, res %= z;

	return r = uint2048_t{res};
}

uint2048_t& rsa2048::enc(const uint2048_t &pt, uint2048_t &r, const uint2048_t &pub)
{
	return pow(pt, E, pub, r);
}

uint2048_t& rsa2048::dec(const uint2048_t &ct, uint2048_t &r, const uint2048_t &pub, const uint2048_t &priv)
{
	return pow(ct, priv, pub, r);
}
