#ifndef __RANDOM_H_
#define __RANDOM_H_

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#ifdef WIN32
#pragma warning(pop)
#endif

namespace opendnp3
{

template<class T>
class Random
{

public:
	Random(T aMin = std::numeric_limits<T>::min(), T aMax = std::numeric_limits<T>::max()) :
		rng(),
		dist(aMin, aMax),
		nextRand(rng, dist) {

	}

	T Next() {
		T ret = nextRand();
		return ret;
	}

private:
	boost::mt19937 rng;
	boost::uniform_int<T> dist;
	boost::variate_generator<boost::mt19937&, boost::uniform_int<T> > nextRand;
};

class RandomBool : private Random<uint32_t>
{
public:
	RandomBool() : Random<uint32_t>(0, 1)
	{}

	bool NextBool() {
		return Next() ? true : false;
	}
};


}

#endif

