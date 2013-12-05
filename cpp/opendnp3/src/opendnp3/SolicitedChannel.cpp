#include "SolicitedChannel.h"

#include "AppLayer.h"
#include "AppChannelStates.h"

#include <openpal/LoggableMacros.h>


namespace opendnp3
{


SolicitedChannel::SolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, IExecutor* apExecutor, openpal::TimeDuration aTimeout) :
	AppLayerChannel("Solicited", aLogger, apApp, apExecutor, aTimeout)
{}

bool SolicitedChannel::AcceptsResponse()
{
	return mpState->AcceptsResponse();
}

void SolicitedChannel::DoSendSuccess()
{
	mpAppLayer->mpUser->OnSolSendSuccess();
}

void SolicitedChannel::DoFailure()
{
	mpAppLayer->mpUser->OnSolFailure();
}

void SolicitedChannel::OnResponse(APDU& arAPDU)
{
	mpState->OnResponse(this, arAPDU);
}

void SolicitedChannel::OnRequest(APDU& arAPDU)
{

	AppControlField acf = arAPDU.GetControl();

	SequenceInfo seq = SI_OTHER;
	if (acf.SEQ == this->Sequence()) {
		LOG_BLOCK(LogLevel::Warning, "Received previous sequence");
		seq = SI_PREV;
	}
	else if (acf.SEQ == NextSeq(this->Sequence())) {
		seq = SI_CORRECT;
	}

	mSequence = acf.SEQ;

	mpAppLayer->mpUser->OnRequest(arAPDU, seq);
}

void SolicitedChannel::OnUnknownObjectInRequest(const AppControlField& acf)
{
	mSequence = acf.SEQ; // capture the sequence number
}

}


