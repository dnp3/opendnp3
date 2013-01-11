//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "Parsing.h"

#include <sstream>
#include <boost/numeric/conversion/converter.hpp>

namespace apl
{

bool Parsing::Get(const std::string& aArg, bool& arValue)
{
	if(aArg == "true") {
		arValue = true;
		return true;
	}
	else if(aArg == "false") {
		arValue = false;
		return true;
	}
	else {
		return Get<bool>(aArg, arValue);
	}
}

bool Parsing::Get(const std::string& aArg, uint8_t& arValue)
{
	int value;
	if(Parsing::Get(aArg, value)) {
		try {
			arValue = boost::numeric::converter<uint8_t, int>::convert(value);
		}
		catch(...) {
			return false;
		}
		return true;
	}
	else return false;
}

}

