
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
#include "LogTypes.h"
#include "Util.h"

#include <sstream>

namespace opendnp3
{

const FilterAssoc LogTypes::filters[LogTypes::NUM_FILTER] = {
	{LEV_DEBUG, 'd'},
	{LEV_COMM, 'c'},
	{LEV_INTERPRET, 'p'},
	{LEV_INFO, 'i'},
	{LEV_WARNING, 'w'},
	{LEV_ERROR, 'e'},
	{LEV_EVENT, 'v'}
};

int LogTypes::FilterLevelToMask(FilterLevel aFilter)
{
	//since FilterLevel is a power of 2 (single bit), subtracting 1 will
	//set all the bits below the set bit.
	//set the filter bit and all the bits below it
	return aFilter | (aFilter - 1);
}

FilterLevel LogTypes::ConvertIntToFilterLevel(int aLevel)
{
	switch(aLevel) {
		case(LEV_DEBUG): return LEV_DEBUG;
		case(LEV_COMM): return LEV_COMM;
		case(LEV_INTERPRET): return LEV_INTERPRET;
		case(LEV_INFO): return LEV_INFO;
		case(LEV_WARNING):return LEV_WARNING;
		case(LEV_ERROR): return LEV_ERROR;
		case(LEV_EVENT): return LEV_EVENT;
		default: return LEV_DEBUG;
	}
}

int LogTypes::GetFilterMask(char c)
{
	switch(c) {
	case('a'): return MASK_ALL_LEVELS;
	case('d'): return LEV_DEBUG;
	case('c'): return LEV_COMM;
	case('p'): return LEV_INTERPRET;
	case('i'): return LEV_INFO;
	case('w'): return LEV_WARNING;
	case('e'): return LEV_ERROR;
	case('v'): return LEV_EVENT;
	case('n'): return 0;
	default:
		return -1;
	}
}

int LogTypes::GetFilterMask(const std::string& arg)
{
	std::string copy(arg);
	toLowerCase(copy);
	int ret = 0;
	for(auto c: copy) ret |= GetFilterMask(c);	
	return ret;
}

std::string LogTypes::GetLevelString( FilterLevel aLevel )
{
	switch(aLevel) {
	case(LEV_DEBUG): return "DEBUG";
	case(LEV_COMM): return "COMM";
	case(LEV_INTERPRET): return "INTERPRET";
	case(LEV_INFO): return "INFO";
	case(LEV_WARNING):return "WARNING";
	case(LEV_ERROR): return "ERROR";
	case(LEV_EVENT): return "EVENT";
	default: return "UNKNOWN";
	}
}

std::string LogTypes::GetFilterString(int aLevel)
{
	std::ostringstream oss;

	oss << "{";

	for(size_t i = 0; i < LogTypes::NUM_FILTER; ++i) {
		oss << static_cast<uint8_t>((aLevel & filters[i].lev) ? filters[i].id : ' ');
	}

	oss << "}";

	return oss.str();
}

}

