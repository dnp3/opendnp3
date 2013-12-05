#include "MockAppUser.h"

#include <sstream>

using namespace std;

namespace opendnp3
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

} //end ns

