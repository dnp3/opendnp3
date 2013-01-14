
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
#include "LinkReceiverStates.h"

#include <APL/Exception.h>
#include <APL/PackingUnpacking.h>
#include <APL/Logger.h>
#include <APL/Location.h>

#include "LinkLayerReceiver.h"
#include "DNPCrc.h"



namespace apl
{
namespace dnp
{

////////////////////////////////////
// Base class
////////////////////////////////////

bool LRS_Base::Parse(LinkLayerReceiver*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

////////////////////////////////////
// Inherited States
////////////////////////////////////

LRS_Sync LRS_Sync::mInstance;

bool LRS_Sync::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < 2) return false;
	else {
		if(c->Sync0564()) c->ChangeState(LRS_Header::Inst());
		return true;
	}
}

LRS_Header LRS_Header::mInstance;

bool LRS_Header::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < 10) return false;
	else {
		if(c->ReadHeader()) {
			c->ChangeState(LRS_Body::Inst());
		}
		else {
			c->FailFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		return true;
	}
}

LRS_Body LRS_Body::mInstance;

bool LRS_Body::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < c->mFrameSize) return false;
	else {
		if(c->ValidateBody()) {
			c->PushFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		else {
			c->FailFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		return true;
	}
}




}
}
