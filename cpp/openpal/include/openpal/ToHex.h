
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
#ifndef __TO_HEX_H_
#define __TO_HEX_H_

#include <openpal/Visibility.h>

#include <cstdint>
#include <string>
#include <sstream>

namespace openpal
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
char DLL_LOCAL toHex(char c);
std::string DLL_LOCAL ByteToHex(uint8_t b);

template<typename T>
std::string DLL_LOCAL toHex(T aBuff, size_t aLength, bool spaced = false)
{
	std::ostringstream oss;
	size_t last = aLength - 1;
	for(size_t i = 0; i < aLength; i++) {
		char c = aBuff[i];
		oss << toHex((c & 0xf0) >> 4) << toHex(c & 0xf);
		if(spaced && i != last)oss << " ";
	}
	return oss.str();
};

template<class T>
std::string DLL_LOCAL TypeToHex(T x)
{
	return toHex(reinterpret_cast<uint8_t*>(&x), sizeof(T), true);
}
#endif

}

#endif
