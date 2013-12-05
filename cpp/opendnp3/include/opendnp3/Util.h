#ifndef __UTIL_H_
#define __UTIL_H_

#include <string>
#include <cstdint>

namespace opendnp3
{

template <class T>
inline T Min(T a, T b)
{
	return (a < b) ? a : b;
}

template <class T>
inline T Max(T a, T b)
{
	return (a > b) ? a : b;
}

template <class T>
bool FloatEqual(T a, T b, T eapllon = 1e-6)
{
	T diff = a - b;
	if(diff < 0) diff = -diff;
	return diff <= eapllon;
}


double SafeCastInt64ToDouble(int64_t aInput);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES

void toUpperCase(std::string& aStr);
void toLowerCase(std::string& aStr);

#endif

}

#endif
