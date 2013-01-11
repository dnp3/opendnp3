
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
#ifndef __DO_TERMINAL_EXTENSION_H_
#define __DO_TERMINAL_EXTENSION_H_

#include "TerminalInterfaces.h"

#include <APL/DataInterfaces.h>
#include <APL/Parsing.h>
#include <APL/ChangeBuffer.h>

#include <queue>
#include <vector>
#include <string>

namespace apl
{
/** Terminal extension that allows a user to manually update points on a data observer.
*/
class DOTerminalExtension : public ITerminalExtension
{
public:
	DOTerminalExtension(IDataObserver* apObserver) : mpObserver(apObserver) {}

	virtual ~DOTerminalExtension() {}

private:

	retcode HandleQueueBinary(std::vector<std::string>& arArgs) {
		return HandleQueue<Binary>(arArgs);
	}

	retcode HandleQueueAnalog(std::vector<std::string>& arArgs) {
		return HandleQueue<Analog>(arArgs);
	}

	retcode HandleQueueCounter(std::vector<std::string>& arArgs) {
		return HandleQueue<Counter>(arArgs);
	}

	template <class T>
	retcode HandleQueue(std::vector<std::string>& arArgs);
	retcode HandleDoTransaction(std::vector<std::string>& arArgs);
	IDataObserver* mpObserver;

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);

	ChangeBuffer mBuffer;
};

template<class T>
bool InterpretAsQualityString(const std::string& aStr, size_t& aQuality)
{
	aQuality = 0;
	for(auto c: aStr) {
		uint8_t m = T::QualConverter::GetMask(c);
		if (m == 0) {
			aQuality = T::ONLINE;
			return false;
		}
		aQuality |= m;
	}
	if (aQuality == 0)
		aQuality = T::ONLINE;
	return true;
}

template <class T>
retcode DOTerminalExtension::HandleQueue(std::vector<std::string>& arArgs)
{
	size_t index;
	typename T::ValueType value;
	size_t quality = AQ_ONLINE; //boost::uint8_t is interpreted as a '1' instead of 0x01

	switch(arArgs.size()) {
	case(3):
		if(!Parsing::Get(arArgs[2], quality)) {
			if (!InterpretAsQualityString<T>(arArgs[2], quality))
				return BAD_ARGUMENTS;
		}
	case(2):
		if(!Parsing::Get(arArgs[0], index)) return BAD_ARGUMENTS;
		if(!Parsing::Get(arArgs[1], value)) return BAD_ARGUMENTS;
		break;
	default:
		return BAD_ARGUMENTS;
	}

	T p(value, static_cast<uint8_t>(quality));
	p.SetToNow();
	Transaction tr(&mBuffer);
	mBuffer.Update(p, index);

	return SUCCESS;
}
}

#endif
