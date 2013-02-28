
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
#include <string>
#include <stdio.h>
#include <stddef.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <vector>

#include <opendnp3/Util.h>
#include <opendnp3/Exception.h>

#ifdef WIN32
#define sscanf sscanf_s
//#define ctime ctime_s
#endif

using namespace std;
using namespace std::chrono;

#include <iomanip>
#include <iterator>

#include <time.h>

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

std::string ToNormalizedString(const timer_clock::time_point& arTime)
{
	std::time_t t = timer_clock::to_time_t(arTime);
	std::string ts = ctime(&t);    // convert to calendar time
	ts.resize(ts.size() - 6);      // skip trailing newline anbd year. TODO - find a better, safer impl!
	ostringstream oss;
	auto us = std::chrono::duration_cast<microseconds>(arTime.time_since_epoch()).count();
	auto fractional = us % (1000 * 1000);
	oss << ts << "." << fractional;
	return oss.str();
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
