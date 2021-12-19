/*
 * File: util.cpp
 * Authors: Alexander Epp, Rain Epp
 * Project: CMPUT274 Final Project
 * Description: Implementation of utility functions
 */

#include "util.h"
#include "vec.h"

namespace util
{
	uint32_t _lcrg_x_;
	// Linear Congruential Random Generator
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	static uint32_t lcrg()
	{
		static const uint32_t a = 1103515245;
		static const uint32_t c = 12345;

		return _lcrg_x_ = a*_lcrg_x_ + c; // Automatic modulus
	}

	// Seed lcrg
	static void slcrg(uint32_t s)
	{
		debugPrint("Seeding lcrg: ", s);
		_lcrg_x_ = s;
	}

	int32_t random(int32_t maxval)
	{
		return lcrg() % maxval;
	}
	void srand(uint32_t seed)
	{
		slcrg(seed);
	}

#ifdef ARDUINO_BUILD
	/*int32_t random(int32_t max)
	{
		return ::random(max);
	}*/
	void srand()
	{
		slcrg(getSeed<int32_t, sizeof(int32_t)>(0, 50));
		//::randomSeed(getSeed<int32_t, sizeof(int32_t)>(0, 50));
	}
#else
	/*int32_t random(int32_t maxval)
	{
			return std::rand()%maxval;
	}*/
	void srand()
	{
		slcrg(std::time(0));
		//std::srand(std::time(0));
	}
#endif
}
