
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
// you under the terms of the License.
//

#include <openpal/Parsing.h>

#include <sstream>
#include <limits>

namespace openpal
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
	uint16_t value;
	if(Parsing::Get(aArg, value)) {

		if(value > std::numeric_limits<uint8_t>::max()) return false;
		else {
			arValue = static_cast<uint8_t>(value);
			return true;
		}
	}
	else return false;
}

}

