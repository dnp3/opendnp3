
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
#ifndef __CONTROL_TASKS_H_
#define __CONTROL_TASKS_H_

#include "MasterTaskBase.h"
#include "ObjectInterfaces.h"
#include "ObjectReadIterator.h"
#include "APDU.h"

#include <functional>

#include <APL/CopyableBuffer.h>
#include <APL/CommandTypes.h>
#include <APL/CommandQueue.h>

namespace apl
{
namespace dnp
{

// Base class with machinery for performing control operations
class ControlTaskBase : public MasterTaskBase
{
public:
	ControlTaskBase(Logger*);
	virtual ~ControlTaskBase() {}

protected:

	// This multi request task requires state
	enum State {
		SELECT,
		OPERATE,
		INVALID
	};

	State mState;
	CommandData mData;

	std::function<CommandStatus (const APDU&)> mValidator;

	template <class T>
	static CommandStatus ValidateCommandResponse(const APDU& arAPDU, CommandObject<T>* apObj, const CopyableBuffer& arData, size_t aIndex);

	bool GetSelectBit();

	// override from base class
	void OnFailure();

private:

	void Respond(CommandStatus aStatus);

	TaskResult _OnPartialResponse(const APDU&);
	TaskResult _OnFinalResponse(const APDU&);
};

// Base class that adds the ConfigureRequest and Set functions.
// Leaves the inherited classes only needing to define the GetObject() function
template <class T>
class ControlTask : public ControlTaskBase
{
public:
	ControlTask(Logger* apLogger) : ControlTaskBase(apLogger)
	{}

	virtual ~ControlTask() {}

	void Set(const T& arCommand, const CommandData& arData, bool aIsSBO) {
		mCommand = arCommand;
		mData = arData;
		mState = aIsSBO ? SELECT : OPERATE;
	}

	void ConfigureRequest(APDU& arAPDU);

protected:

	virtual CommandObject<T>* GetObject(const T& arCmd) = 0;

	T mCommand;
};

// Concrete class for BinaryOutput commands
class BinaryOutputTask : public ControlTask<BinaryOutput>
{
public:
	BinaryOutputTask(Logger*);

	CommandObject<BinaryOutput>* GetObject(const BinaryOutput&);

	std::string Name() const {
		return "BinaryOutputTask";
	}
};

// Concrete class for Setpoint commands
class SetpointTask : public ControlTask<Setpoint>
{
public:
	SetpointTask(Logger*);

	CommandObject<Setpoint>* GetObject(const Setpoint&);

	std::string Name() const {
		return "SetpointTask";
	}

	static CommandObject<Setpoint>* GetOptimalEncoder(SetpointEncodingType aType);
};

template <class T>
void ControlTask<T>::ConfigureRequest(APDU& arAPDU)
{
	CommandObject<T>* pObj = this->GetObject(mCommand);
	arAPDU.Set(this->GetSelectBit() ? FC_SELECT : FC_OPERATE, true, true, false, false);
	IndexedWriteIterator i = arAPDU.WriteIndexed(pObj, 1, mData.mIndex);
	i.SetIndex(mData.mIndex);
	pObj->Write(*i, mCommand);
	CopyableBuffer buffer = pObj->GetValueBytes(*i);
	mValidator = std::bind(&ControlTaskBase::ValidateCommandResponse<T>, std::placeholders::_1, pObj, buffer, mData.mIndex);
}

template <class T>
CommandStatus ControlTaskBase::ValidateCommandResponse(const APDU& arAPDU, CommandObject<T>* apObj, const CopyableBuffer& arData, size_t aIndex)
{
	HeaderReadIterator hdr = arAPDU.BeginRead();
	if(hdr.Count() == 1) {

		int grp = hdr->GetGroup();
		int var = hdr->GetVariation();

		if(grp == apObj->GetGroup() && var == apObj->GetVariation()) {
			ObjectReadIterator obj = hdr.BeginRead();
			if(obj.Count() == 1 && obj->Index() == aIndex) { //compare what was written to what was received
				T cmd = apObj->Read(*obj);
				if(arData == apObj->GetValueBytes(*obj)) return cmd.mStatus;
				else return CS_FORMAT_ERROR;
			}
		}
	}

	return CS_UNDEFINED;
}



}
} //ens ns

#endif
