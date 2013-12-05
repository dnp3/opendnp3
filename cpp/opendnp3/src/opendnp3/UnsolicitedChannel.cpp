#include "UnsolicitedChannel.h"

#include <openpal/LoggableMacros.h>

#include "AppLayer.h"

namespace opendnp3
{


UnsolicitedChannel::UnsolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, IExecutor* apExecutor, openpal::TimeDuration aTimeout) :
	AppLayerChannel("Unsolicited", aLogger, apApp, apExecutor, aTimeout)
{}

void UnsolicitedChannel::OnUnsol(APDU& arAPDU)
{
	AppControlField acf = arAPDU.GetControl();

	if(acf.SEQ == mSequence) {
		LOG_BLOCK(LogLevel::Info, "Ignoring repeat unsol seq: " << acf.SEQ)
	}
	else { // only process the data if the sequence number is new
		mSequence = acf.SEQ;
		mpAppLayer->mpUser->OnUnsolResponse(arAPDU);
	}
}

void UnsolicitedChannel::DoSendSuccess()
{
	mpAppLayer->mpUser->OnUnsolSendSuccess();
}

void UnsolicitedChannel::DoFailure()
{
	mpAppLayer->mpUser->OnUnsolFailure();
}

}



