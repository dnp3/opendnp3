#ifndef __RANDOM_DOUBLE_H_
#define __RANDOM_DOUBLE_H_

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#ifdef WIN32
#pragma warning(pop)
#endif

namespace opendnp3
{

class RandomDouble
{

public:
	RandomDouble() :
		rng(),
		dist(0.0, 1.0),
		nextRand(rng, dist) {

	}

	double Next() {
		return nextRand();
	}

private:
	boost::mt19937 rng;
	boost::uniform_real<double> dist;
	boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > nextRand;
};

}

#endif

