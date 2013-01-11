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
#include "MockAppUser.h"

#include <sstream>

using namespace std;

namespace apl
{
namespace dnp
{

ostream& operator<<(ostream& output, const MockAppUser::State& s)
{
	output << " LayerUp: " << s.NumLayerUp
	       << " LayerDown: " << s.NumLayerDown
	       << " Unsol: " << s.NumUnsol
	       << " SolSendSuccess: " << s.NumSolSendSuccess
	       << " SolFailure: " << s.NumSolFailure
	       << " UnsolSendSuccess: " << s.NumUnsolSendSuccess
	       << " UnsolFailure: " << s.NumUnsolFailure
	       << " PartialResponse: " << s.NumPartialRsp
	       << " FinalRsp: " << s.NumFinalRsp
	       << " Request: " << s.NumRequest
	       << " Unknown: " << s.NumUnknown;

	return output;
}


MockAppUser::State::State() :
	NumLayerUp(0),
	NumLayerDown(0),
	NumUnsol(0),
	NumSolSendSuccess(0),
	NumSolFailure(0),
	NumUnsolSendSuccess(0),
	NumUnsolFailure(0),
	NumPartialRsp(0),
	NumFinalRsp(0),
	NumRequest(0),
	NumUnknown(0)
{}

bool MockAppUser::State::operator==(const State& arState) const
{
	return	this->NumLayerUp == arState.NumLayerUp &&
	        this->NumLayerDown == arState.NumLayerDown &&
	        this->NumUnsol == arState.NumUnsol &&
	        this->NumSolSendSuccess == arState.NumSolSendSuccess &&
	        this->NumSolFailure == arState.NumSolFailure &&
	        this->NumUnsolSendSuccess == arState.NumUnsolSendSuccess &&
	        this->NumUnsolFailure == arState.NumUnsolFailure &&
	        this->NumPartialRsp == arState.NumPartialRsp &&
	        this->NumFinalRsp == arState.NumFinalRsp &&
	        this->NumRequest == arState.NumRequest &&
	        this->NumUnknown == arState.NumUnknown;
}

MockAppUser::MockAppUser(bool aIsMaster) :
	mIsMaster(aIsMaster)
{}

bool MockAppUser::IsMaster()
{
	return mIsMaster;
}

// Implement IAppUser
void MockAppUser::OnLowerLayerUp()
{
	++mState.NumLayerUp;
}

void MockAppUser::OnLowerLayerDown()
{
	++mState.NumLayerDown;
}

void MockAppUser::OnSolSendSuccess()
{
	++mState.NumSolSendSuccess;
}

void MockAppUser::OnSolFailure()
{
	++mState.NumSolFailure;
}

void MockAppUser::OnUnsolSendSuccess()
{
	++mState.NumUnsolSendSuccess;
}

void MockAppUser::OnUnsolFailure()
{
	++mState.NumUnsolFailure;
}

void MockAppUser::OnPartialResponse(const APDU&)
{
	++mState.NumPartialRsp;
}

void MockAppUser::OnFinalResponse(const APDU&)
{
	++mState.NumFinalRsp;
}

void MockAppUser::OnUnsolResponse(const APDU&)
{
	++mState.NumUnsol;
}

void MockAppUser::OnRequest(const APDU&, SequenceInfo)
{
	++mState.NumRequest;
}

void MockAppUser::OnUnknownObject()
{
	++mState.NumUnknown;
}

bool MockAppUser::Equals(const MockAppUser::State& arState) const
{
	return mState == arState;
}

}
} //#end ns
