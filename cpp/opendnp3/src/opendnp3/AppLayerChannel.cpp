#include "AppLayerChannel.h"

#include <openpal/LoggableMacros.h>
#include <openpal/IExecutor.h>

#include "AppLayer.h"
#include "AppChannelStates.h"

#include <functional>

using namespace openpal;

namespace opendnp3
{

AppLayerChannel::AppLayerChannel(const std::string& arName, openpal::Logger& arLogger, AppLayer* apAppLayer, IExecutor* apExecutor, TimeDuration aTimeout) :
	Loggable(arLogger),
	mpAppLayer(apAppLayer),
	mpSendAPDU(nullptr),
	mNumRetry(0),
	mpExecutor(apExecutor),
	mpTimer(nullptr),
	M_TIMEOUT(aTimeout),
	M_NAME(arName)
{
	this->Reset();
}

void AppLayerChannel::Reset()
{
	mpState = ACS_Idle::Inst();
	mConfirming = false;
	mSequence = -1;
	if(mpTimer) {
		mpTimer->Cancel();
		mpTimer = nullptr;
	}
}

// ---- Events ----

void AppLayerChannel::Send(APDU& arAPDU, size_t aNumRetry)
{
	mpState->Send(this, arAPDU, aNumRetry);
	mpSendAPDU = &arAPDU;
}

void AppLayerChannel::OnSendSuccess()
{
	mpState->OnSendSuccess(this);
}

void AppLayerChannel::OnSendFailure()
{
	mpState->OnSendFailure(this);
}

void AppLayerChannel::OnConfirm(int aSeq)
{
	mpState->OnConfirm(this, aSeq);
}

void AppLayerChannel::Cancel()
{
	mpState->Cancel(this);
}


// ---- State Actions ----

void AppLayerChannel::QueueSend(APDU& arAPDU)
{
	mpAppLayer->QueueFrame(arAPDU);
}

bool AppLayerChannel::Retry(ACS_Base* apState)
{
	if(mNumRetry > 0) {
		--mNumRetry;
		LOG_BLOCK(LogLevel::Info, "App layer retry, " << mNumRetry << " remaining");
		this->ChangeState(apState);
		mpAppLayer->QueueFrame(*mpSendAPDU);
		return true;
	}
	else {
		return false;
	}
}

void AppLayerChannel::DoPartialResponse(APDU& arAPDU)
{
	mpAppLayer->mpUser->OnPartialResponse(arAPDU);
}

void AppLayerChannel::DoFinalResponse(APDU& arAPDU)
{
	mpAppLayer->mpUser->OnFinalResponse(arAPDU);
}

void AppLayerChannel::StartTimer()
{
	assert(mpTimer == nullptr);
	mpTimer = mpExecutor->Start(TimeDuration(M_TIMEOUT), std::bind(&AppLayerChannel::Timeout, this));
}

void AppLayerChannel::CancelTimer()
{
	assert(mpTimer != nullptr);
	mpTimer->Cancel();
	mpTimer = nullptr;
}

void AppLayerChannel::ChangeState(ACS_Base* apState)
{
	if(apState != mpState) {
		LOG_BLOCK(LogLevel::Debug, "State changed from " << mpState->Name() << " to " << apState->Name());
		mpState = apState;
	}
}

void AppLayerChannel::Timeout()
{
	mpTimer = nullptr;
	mpState->OnTimeout(this);
}

}


