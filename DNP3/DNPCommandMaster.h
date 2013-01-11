
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
#ifndef __DNP_COMMAND_MASTER_H_
#define __DNP_COMMAND_MASTER_H_


#include <APL/CommandInterfaces.h>
#include <APL/Exception.h>
#include <APL/ITimeSource.h>
#include <APL/Location.h>
#include <DNP3/APDUConstants.h>
#include <DNP3/AppInterfaces.h>
#include <map>
#include <assert.h>

namespace apl
{
namespace dnp
{

struct DeviceTemplate;

/** Struct that characterizes command requests and provides all information necessary for validation.
*/
template <class T>
struct CommandRequestInfo {
	CommandRequestInfo() : group(0), var(0), qual(QC_UNDEFINED), seqInfo(SI_OTHER), seqAPDU(-1) {}
	CommandRequestInfo(const T& aCmd, int aGroup, int aVar, QualifierCode aQual, SequenceInfo aSeqInfo, int aSeqAPDU)
		: cmd(aCmd), group(aGroup), var(aVar), qual(aQual), seqInfo(aSeqInfo), seqAPDU(aSeqAPDU) {}

	T cmd;
	int group;
	int var;
	QualifierCode qual;
	SequenceInfo seqInfo;
	int seqAPDU;

	bool operator==(const CommandRequestInfo<T>& rhs) const {
		return (rhs.cmd == cmd) && (rhs.group == group) && (rhs.var == var) && (rhs.qual == qual);
	}
	bool operator!=(const CommandRequestInfo<T>& rhs) const {
		return !(*this == rhs);
	}
};

class IDNPCommandMaster
{
public:
	virtual ~IDNPCommandMaster();
	virtual void DeselectAll() = 0;

	virtual CommandStatus DirectOperate(const CommandRequestInfo<BinaryOutput>&, size_t, int) = 0;
	virtual CommandStatus DirectOperate(const CommandRequestInfo<Setpoint>&, size_t, int) = 0;
	virtual CommandStatus Operate(const CommandRequestInfo<BinaryOutput>&, size_t, int) = 0;
	virtual CommandStatus Operate(const CommandRequestInfo<Setpoint>&, size_t, int) = 0;
	virtual bool Select(const CommandRequestInfo<Setpoint>& aCmd, size_t aIndex) = 0;
	virtual bool Select(const CommandRequestInfo<BinaryOutput>& aCmd, size_t aIndex) = 0;

	virtual void SetResponseObserver(apl::IResponseAcceptor* apAcceptor) = 0;
};

/**
	Handles DNP3 control/setpoint behavior, using a DeviceTemplate for start-up characterization of binary and analog outputs. Keeps
	a map to match operates to previous selects, with a separate control path for direct operates. If command requests are validated
	they are passed to the user code through ICommandAcceptor, which responds with success/failure.

*/
class DNPCommandMaster: public IDNPCommandMaster, public apl::IResponseAcceptor
{

	template <typename T>
	struct CommandInfo {
		CommandInfo(): mSelectTime(0) {}
		size_t mRemoteIndex;
		apl::ICommandAcceptor* mpAcceptor;		
		CommandModes mMode;
		apl::millis_t mSelectTime;
		bool mIsSelected;
		millis_t mSelectTimeoutMS;
		CommandRequestInfo<T> mRequest;
		int mSelectSequence;
	};

	typedef std::map<size_t, CommandInfo<BinaryOutput> > ControlMap;
	typedef std::map<size_t, CommandInfo<Setpoint> > SetpointMap;

public:

	DNPCommandMaster(apl::ITimeSource* apTimeSource, apl::millis_t aSelectTimeout, CommandModes aMode = CM_SBO_ONLY);

	void Configure(const DeviceTemplate& arTmp, apl::ICommandAcceptor* apAcceptor);

	//Implement the IDNPCommandMaster interface
	void DeselectAll();
	bool Select(const CommandRequestInfo<Setpoint>& aCmd, size_t aIndex);
	bool Select(const CommandRequestInfo<BinaryOutput>& aCmd, size_t aIndex);
	void SetResponseObserver(IResponseAcceptor* apAcceptor);

	CommandStatus Operate(const CommandRequestInfo<BinaryOutput>&, size_t, int);
	CommandStatus Operate(const CommandRequestInfo<Setpoint>&, size_t, int);

	CommandStatus DirectOperate(const CommandRequestInfo<BinaryOutput>&, size_t, int);
	CommandStatus DirectOperate(const CommandRequestInfo<Setpoint>&, size_t, int);

	//Implement the ResponseAcceptor interface
	void AcceptResponse(const apl::CommandResponse&, int aSequence);

	void BindCommand(apl::CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, CommandModes aMode, millis_t aSelectTimeoutMS, apl::ICommandAcceptor* apAcceptor);
	void BindCommand(CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, ICommandAcceptor* apAcceptor);

private:
	template <typename T>
	bool Select(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& aCmd, size_t aIndex);

	template <typename T>
	CommandStatus Operate(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& arType, size_t aIndex, int aSequence);

	template <typename T>
	CommandStatus DirectOperate(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& arType, size_t aIndex, int aSequence);

	template <typename T>
	void BindCommand(std::map<size_t, CommandInfo<T> >& aMap, apl::CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, CommandModes aMode, millis_t aSelectTimeoutMS, apl::ICommandAcceptor* apAcceptor);

	apl::millis_t mSelectTimeout;
	SetpointMap mSetpointMap;
	ControlMap mControlMap;
	apl::IResponseAcceptor* mpRspAcceptor;
	apl::ITimeSource* mpTimeSource;
	CommandModes mCommandMode;
};


template <typename T>
void DNPCommandMaster::BindCommand(std::map<size_t, CommandInfo<T> >& aMap, apl::CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, CommandModes aMode, millis_t aSelectTimeoutMS, apl::ICommandAcceptor* apAcceptor)
{
	std::pair<size_t, CommandInfo<T> > mapping;
	mapping.first = aLocalIndex;
	mapping.second.mMode = aMode;
	mapping.second.mpAcceptor = apAcceptor;
	mapping.second.mRemoteIndex = aRemoteIndex;
	mapping.second.mSelectTimeoutMS = aSelectTimeoutMS;

	if(!aMap.insert(mapping).second) {
		throw Exception(LOCATION, "Command is not unique");
	}
}

inline bool DNPCommandMaster::Select(const CommandRequestInfo<BinaryOutput>& aCmd, size_t aIndex)
{
	return this->Select<apl::BinaryOutput>(mControlMap, aCmd, aIndex);
}
inline bool DNPCommandMaster::Select(const CommandRequestInfo<Setpoint>& aCmd, size_t aIndex)
{
	return this->Select<apl::Setpoint>(mSetpointMap, aCmd, aIndex);
}

template <typename T>
bool DNPCommandMaster::Select(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& aCmd, size_t aIndex)
{
	assert(mpRspAcceptor != NULL);
	typename std::map<size_t, CommandInfo<T> >::iterator i = aMap.find(aIndex);
	if(i == aMap.end()) return false;
	else {
		CommandInfo<T>& info = i->second;

		if(info.mMode == CM_DO_ONLY) return false;

		info.mSelectTime = mpTimeSource->GetMillisecondsSinceEpoch();
		info.mIsSelected = true;
		info.mSelectSequence = aCmd.seqAPDU;
		info.mRequest = aCmd;

		return true;
	}
}

inline CommandStatus DNPCommandMaster::Operate(const CommandRequestInfo<BinaryOutput>& arType, size_t aIndex, int aSequence)
{
	return this->Operate<apl::BinaryOutput>(mControlMap, arType, aIndex, aSequence);
}
inline CommandStatus DNPCommandMaster::Operate(const CommandRequestInfo<Setpoint>& arType, size_t aIndex, int aSequence)
{
	return this->Operate<apl::Setpoint>(mSetpointMap, arType, aIndex, aSequence);
}

template <typename T>
CommandStatus DNPCommandMaster::Operate(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& arType, size_t aIndex, int aSequence)
{
	assert(mpRspAcceptor != NULL);
	typename std::map<size_t, CommandInfo<T> >::iterator i = aMap.find(aIndex);
	if(i == aMap.end()) return CS_NOT_SUPPORTED;
	else {
		CommandInfo<T>& info = i->second;

		if(!info.mIsSelected) {
			return CS_NO_SELECT;
		}
		else if(mpTimeSource->GetMillisecondsSinceEpoch() - info.mSelectTime > mSelectTimeout) {
			return CS_TIMEOUT;
		}

		if ( (arType.seqAPDU != ((info.mSelectSequence + 1) % 16)) || info.mRequest != arType ) {
			info.mIsSelected = false;
			return CS_NO_SELECT;
		}

		info.mIsSelected = false;
		info.mpAcceptor->AcceptCommand(arType.cmd, info.mRemoteIndex, aSequence, this);
		return CS_SUCCESS;
	}
}

inline CommandStatus DNPCommandMaster::DirectOperate(const CommandRequestInfo<BinaryOutput>& arType, size_t aIndex, int aSequence)
{
	return this->DirectOperate<apl::BinaryOutput>(mControlMap, arType, aIndex, aSequence);
}
inline CommandStatus DNPCommandMaster::DirectOperate(const CommandRequestInfo<Setpoint>& arType, size_t aIndex, int aSequence)
{
	return this->DirectOperate<apl::Setpoint>(mSetpointMap, arType, aIndex, aSequence);
}

template <typename T>
CommandStatus DNPCommandMaster::DirectOperate(std::map<size_t, CommandInfo<T> >& aMap, const CommandRequestInfo<T>& arType, size_t aIndex, int aSequence)
{
	assert(mpRspAcceptor != NULL);
	typename std::map<size_t, CommandInfo<T> >::iterator i = aMap.find(aIndex);
	if(i == aMap.end()) return CS_NOT_SUPPORTED;
	else {
		CommandInfo<T>& info = i->second;

		if(info.mMode == CM_SBO_ONLY)
			return CS_NOT_SUPPORTED;

		info.mIsSelected = false;
		info.mpAcceptor->AcceptCommand(arType.cmd, info.mRemoteIndex, aSequence, this);
		return CS_SUCCESS;
	}
}



}
}

#endif
