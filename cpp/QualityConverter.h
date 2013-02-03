
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
#ifndef __QUALITY_CONVERTER_H_
#define __QUALITY_CONVERTER_H_

#include "QualityMasks.h"
#include "Types.h"

#include <string>
#include <sstream>

namespace opendnp3
{

template <class T, size_t N>
struct QualityInfo {
	const static size_t NUM = N;
	const static uint8_t masks[N];
	const static std::string names[N];
	const static char symbols[N];
};

typedef QualityInfo<BinaryQuality, 6> BinaryQualInfo;
typedef QualityInfo<AnalogQuality, 7> AnalogQualInfo;
typedef QualityInfo<CounterQuality, 7> CounterQualInfo;
typedef QualityInfo<ControlQuality, 5> ControlQualInfo;
typedef QualityInfo<SetpointQuality, 4> SetpointQualInfo;
typedef QualityInfo<VtoQuality, 4> VtoQualInfo;					/* TODO - what is the proper N value? */

template <class T>
class QualityConverter
{
public:

	static char GetSymbol(uint8_t aMask) {
		for (size_t i = 0; i < T::NUM; ++i)
			if(T::masks[i] == aMask) return T::symbols[i];
		return '.';
	}

	static std::string GetName(uint8_t aMask) {
		for (size_t i = 0; i < T::NUM; ++i)
			if(T::masks[i] == aMask) return T::names[i];
		return "Reserved";
	}

	static uint8_t GetMask(char aSymbol) {
		for (size_t i = 0; i < T::NUM; ++i)
			if(T::symbols[i] == aSymbol) return T::masks[i];
		return 0;
	}

	static std::string GetSymbolString(uint8_t aQual) {
		std::ostringstream oss;
		oss << "{";
		for (size_t i = 0; i < 8; ++i) {
			if(i < T::NUM && (aQual & T::masks[i])) oss << T::symbols[i];
			else oss << ' ';
		}
		oss << "}";
		return oss.str();
	}

	static std::string GetNameString(uint8_t aQual) {
		std::ostringstream oss;
		for (size_t i = 0; i < T::NUM; ++i)
			if(aQual & T::masks[i]) oss << " " << T::names[i];
		return oss.str();
	}

	static std::string GetAllSymbols() {
		return GetSymbolString(~0);
	}
};

}

#endif
