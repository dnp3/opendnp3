
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
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

namespace apl
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

