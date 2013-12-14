/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __COMMAND_HELPERS_H_
#define __COMMAND_HELPERS_H_

#include <opendnp3/ObjectInterfaces.h>


#include "APDU.h"
#include "ObjectReadIterator.h"

#include <functional>

namespace opendnp3
{

// Static helper functions for formatting/validating command requests
class CommandHelpers
{
public:
	template <class T>
	static std::function<CommandStatus (const openpal::ReadOnlyBuffer&)> ConfigureRequest(APDU& arAPDU, FunctionCode aCode, const T& arCommand, size_t aIndex, CommandObject<T>* apObj);

private:
	template <class T>
	static CommandStatus ValidateCommandResponse(const APDU& arAPDU, CommandObject<T>* apObj, const CopyableBuffer& arData, size_t aIndex);
};

template <class T>
std::function<CommandStatus (const openpal::ReadOnlyBuffer&)> CommandHelpers::ConfigureRequest(APDU& arAPDU, FunctionCode aCode, const T& arCommand, size_t aIndex, CommandObject<T>* apObj)
{
	arAPDU.Set(aCode, true, true, false, false);
	IndexedWriteIterator i = arAPDU.WriteIndexed(apObj, 1, aIndex);
	i.SetIndex(aIndex);
	apObj->Write(*i, arCommand);
	CopyableBuffer buffer = apObj->GetValueBytes(*i);	
	return [](const openpal::ReadOnlyBuffer& rspHeader) {
		return CommandStatus::SUCCESS; // TODO - do a real comparison
	};
}

}

#endif
