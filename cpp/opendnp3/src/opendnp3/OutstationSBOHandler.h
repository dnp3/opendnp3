
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

#ifndef __OUTSTATION_SBO_HANDLER_H_
#define __OUTSTATION_SBO_HANDLER_H_

#include <openpal/Types.h>
#include <openpal/Location.h>
#include <openpal/TimeDuration.h>
#include <openpal/IExecutor.h>

#include <opendnp3/ICommandHandler.h>
#include <opendnp3/APDUConstants.h>

#include <map>

namespace opendnp3
{

class DLL_LOCAL OutstationSBOHandler
{

	template <class T>
	class SelectInfo
	{
	public:
		SelectInfo(const T& arCommand, uint8_t aSequence, QualifierCode aCode, openpal::MonotonicTimestamp aTimestamp) :
			mCommand(arCommand),
			mSequence(aSequence),
			mCode(aCode),
			mTimestamp(aTimestamp),
			mOperated(false)
		{}

		SelectInfo() :
			mSequence(0),
			mCode(QC_UNDEFINED),
			mTimestamp(),
			mOperated(false)
		{}

		T mCommand;
		uint8_t mSequence;
		QualifierCode mCode;
		openpal::MonotonicTimestamp mTimestamp;
		bool mOperated;
	};

	typedef std::map<size_t, SelectInfo<ControlRelayOutputBlock>> CROBSelectMap;
	typedef std::map<size_t, SelectInfo<AnalogOutputInt16>> Analog16SelectMap;
	typedef std::map<size_t, SelectInfo<AnalogOutputInt32>> Analog32SelectMap;
	typedef std::map<size_t, SelectInfo<AnalogOutputFloat32>> AnalogFloatSelectMap;
	typedef std::map<size_t, SelectInfo<AnalogOutputDouble64>> AnalogDoubleSelectMap;

public:
	OutstationSBOHandler(openpal::TimeDuration aSelectTimeout, ICommandHandler* apCmdHandler, openpal::IExecutor* apExecutor);

	CommandStatus Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);
	CommandStatus Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);

	CommandStatus Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);
	CommandStatus Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);

	CommandStatus Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);
	CommandStatus Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);

	CommandStatus Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);
	CommandStatus Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);

	CommandStatus Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);
	CommandStatus Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode);

private:

	void ClearAll();

	template <class T>
	CommandStatus Select(const T& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode, std::map<size_t, SelectInfo<T>>& arMap);

	template <class T>
	CommandStatus Operate(const T& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode, std::map<size_t, SelectInfo<T>>& arMap);

	openpal::TimeDuration mSelectTimeout;
	ICommandHandler* mpCmdHandler;
	openpal::IExecutor* mpExecutor;
	uint8_t mCurrentSequenceNum;

	CROBSelectMap mCROBSelectMap;
	Analog16SelectMap mAnalog16SelectMap;
	Analog32SelectMap mAnalog32SelectMap;
	AnalogFloatSelectMap mAnalogFloatSelectMap;
	AnalogDoubleSelectMap mAnalogDoubleSelectMap;
};

template <class T>
CommandStatus OutstationSBOHandler::Select(const T& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode, std::map<size_t, SelectInfo<T>>& arMap)
{
	if(aSequence != mCurrentSequenceNum) {
		this->ClearAll();
	}

	mCurrentSequenceNum = aSequence;

	CommandStatus status =  mpCmdHandler->Select(arCommand, aIndex);
	if(status == CS_SUCCESS) { //outstation supports this point
		auto time = mpExecutor->GetTime();
		SelectInfo<T> info(arCommand, aSequence, aCode, time);
		arMap[aIndex] = info; // record the select by index
	}
	return status;
}

template <class T>
CommandStatus OutstationSBOHandler::Operate(const T& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode, std::map<size_t, SelectInfo<T>>& arMap)
{
	auto iter = arMap.find(aIndex);
	if(iter == arMap.end()) {
		this->ClearAll();
		return CS_NO_SELECT; //no prior select
	}
	else {
		// what should the sequence number be?
		uint8_t expectedSeq = (iter->second.mSequence + 1) % 16;
		// are all values what we expect them to be?
		if(expectedSeq == aSequence && aCode == iter->second.mCode && arCommand == iter->second.mCommand) {
			// now check the timestamp
			auto now = mpExecutor->GetTime();
			if((now.milliseconds - iter->second.mTimestamp.milliseconds) < mSelectTimeout.GetMilliseconds()) {
				if(iter->second.mOperated) {
					return CS_SUCCESS;
				}
				else {
					iter->second.mOperated = true;
					return mpCmdHandler->Operate(arCommand, aIndex);
				}
			}
			else return CS_TIMEOUT;
		}
		else {
			this->ClearAll();
			return CS_NO_SELECT;
		}
	}
}

}

#endif
