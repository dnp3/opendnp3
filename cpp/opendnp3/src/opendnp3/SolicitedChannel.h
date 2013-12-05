#ifndef __SOLICITED_CHANNEL_H_
#define __SOLICITED_CHANNEL_H_

#include "AppLayerChannel.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class DLL_LOCAL SolicitedChannel : public AppLayerChannel
{
public:
	SolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, openpal::IExecutor* apExecutor, openpal::TimeDuration aTimeout);
	virtual ~SolicitedChannel() {}

	// Called when the app layer has a problem parsing an object header
	void OnUnknownObjectInRequest(const AppControlField& acf);
	void OnResponse(APDU& arAPDU);
	void OnRequest(APDU& arAPDU);

	bool AcceptsResponse();

private:

	// implement virtual memebers from AppLayerChannel
	void DoSendSuccess();
	void DoFailure();
};

}

#endif

