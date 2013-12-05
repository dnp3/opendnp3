#include <string>
#include <stdio.h>
#include <stddef.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <vector>

#include <opendnp3/Util.h>
#include <openpal/Exception.h>

#ifdef WIN32
#define sscanf sscanf_s
//#define ctime ctime_s
#endif

using namespace std;

#include <iomanip>
#include <iterator>

namespace opendnp3
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string Month(int aMonth)
{
	switch(aMonth) {
	case(1): return	"Jan";
	case(2): return "Feb";
	case(3): return "Mar";
	case(4): return "Apr";
	case(5): return "May";
	case(6): return "Jun";
	case(7): return "Jul";
	case(8): return "Aug";
	case(9): return "Sep";
	case(10): return "Oct";
	case(11): return "Nov";
	case(12): return "Dec";
	default: return "Unknown month";
	}
}

#endif

double SafeCastInt64ToDouble(int64_t aInput)
{
#if OPENDNP3_ARM_FLOAT_WORKAROUND
	// fix for arm compiler bug
	long upper =  (aInput & 0xFFFFFFFF00000000LL) >> 32;
	unsigned long lower = (aInput & 0x00000000FFFFFFFFLL);

	//Shift the upper portion back into the correct range ( e1<<e2 becomes e1 * pow(2.0, e2), in our case pow(2,32)=4294967296)
	double f1 = upper * 4294967296.0;
	double f2 = lower;
	double dblVar = f1 + f2;

	return dblVar;
#else
	return static_cast<double>(aInput);
#endif
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
void toUpperCase(std::string& apStr)
{
	//from http://gethelp.devx.com/techtips/cpp_pro/10min/2002/Oct/10min1002-2.asp
	std::transform(apStr.begin(), apStr.end(), apStr.begin(), (int(*)(int)) toupper);
}
void toLowerCase(std::string& apStr)
{
	//from http://gethelp.devx.com/techtips/cpp_pro/10min/2002/Oct/10min1002-2.asp
	std::transform(apStr.begin(), apStr.end(), apStr.begin(), (int(*)(int)) tolower);
}
#endif

}
