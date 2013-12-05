#ifndef __GET_KEYS_H_
#define __GET_KEYS_H_

#include <vector>
#include <openpal/Visibility.h>

namespace opendnp3
{

template <class T, class U>
static std::vector<U> DLL_LOCAL GetKeys(const T& arMap)
{
	std::vector<U> ret;
for(auto pair: arMap) {
		ret.push_back(pair.first);
	}
	return ret;
}

}

#endif

