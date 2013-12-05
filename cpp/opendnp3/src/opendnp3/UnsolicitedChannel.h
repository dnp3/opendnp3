#ifndef __UNSOLICITED_CHANNEL_H_
#define __UNSOLICITED_CHANNEL_H_

#include "AppLayerChannel.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class DLL_LOCAL UnsolicitedChannel : public AppLayerChannel
{
public:
	UnsolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, openpal::IExecutor* apExecutor, openpal::TimeDuration aTimeout);
	virtual ~UnsolicitedChannel() {}

	void OnUnsol(APDU& arAPDU);

	void DoSendSuccess();
	void DoFailure();
};

}

#endif

