
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
#include "DNPCommandMaster.h"

#include <APL/Location.h>
#include <APL/Exception.h>

#include "DeviceTemplate.h"

#include <assert.h>

using namespace std;

namespace apl
{
namespace dnp
{

IDNPCommandMaster::~IDNPCommandMaster() {}

////////////////////////////////////////
// DNPCommandMaster
////////////////////////////////////////

DNPCommandMaster::DNPCommandMaster(apl::ITimeSource* apTimeSource, apl::millis_t aSelectTimeout, CommandModes aMode) :
	mSelectTimeout(aSelectTimeout),
	mpRspAcceptor(NULL),
	mpTimeSource(apTimeSource),
	mCommandMode(aMode)
{

}

void DNPCommandMaster::Configure(const DeviceTemplate& arTmp, ICommandAcceptor* apAcceptor)
{
	for(size_t j = 0; j < arTmp.mControls.size(); ++j) {
		this->BindCommand(CT_BINARY_OUTPUT, j, j, arTmp.mControls[j].CommandMode, arTmp.mControls[j].SelectTimeoutMS, apAcceptor);
	}

	for(size_t j = 0; j < arTmp.mSetpoints.size(); ++j) {
		this->BindCommand(CT_SETPOINT, j, j, arTmp.mSetpoints[j].CommandMode, arTmp.mSetpoints[j].SelectTimeoutMS, apAcceptor);
	}
}

void DNPCommandMaster::DeselectAll()
{
	for(auto pair: mSetpointMap) pair.second.mIsSelected = false;
	for(auto pair: mControlMap) pair.second.mIsSelected = false;
}

void DNPCommandMaster::SetResponseObserver(IResponseAcceptor* apAcceptor)
{
	assert(mpRspAcceptor == NULL);
	mpRspAcceptor = apAcceptor;
}

//Implement the ResponseAcceptor interface
void DNPCommandMaster::AcceptResponse(const apl::CommandResponse& arResponse, int aSequence)
{
	assert(mpRspAcceptor != NULL);
	mpRspAcceptor->AcceptResponse(arResponse, aSequence);
}

void DNPCommandMaster::BindCommand(CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, ICommandAcceptor* apAcceptor)
{
	BindCommand(aType, aLocalIndex, aRemoteIndex, mCommandMode, 5000, apAcceptor);
}
void DNPCommandMaster::BindCommand(CommandTypes aType, size_t aLocalIndex, size_t aRemoteIndex, CommandModes aMode, millis_t aSelectTimeoutMS, ICommandAcceptor* apAcceptor)
{
	assert(apAcceptor != NULL);

	if ( aType == CT_BINARY_OUTPUT )
		BindCommand<BinaryOutput>(mControlMap, aType, aLocalIndex, aRemoteIndex, aMode, aSelectTimeoutMS, apAcceptor);
	else if ( aType == CT_SETPOINT )
		BindCommand<Setpoint>(mSetpointMap, aType, aLocalIndex, aRemoteIndex, aMode, aSelectTimeoutMS, apAcceptor);
	else
		throw Exception(LOCATION, "Command type invalid");
}

}
}
