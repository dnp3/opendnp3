#ifndef __APP_CONFIG_H_
#define __APP_CONFIG_H_

#include "DNPConstants.h"

using namespace openpal;

namespace opendnp3
{

/** Configuration structure for the dnp3 application layer
*/
struct AppConfig {

	/// Default constructor
	AppConfig() : RspTimeout(TimeDuration::Seconds(5)), NumRetry(0), FragSize(DEFAULT_FRAG_SIZE) {}

	AppConfig(TimeDuration aRspTimeout, size_t aNumRetry = 0, size_t aFragSize = DEFAULT_FRAG_SIZE) :
		RspTimeout(aRspTimeout),
		NumRetry(aNumRetry),
		FragSize(aFragSize)
	{}

	/// The response/confirm timeout in millisec
	TimeDuration RspTimeout;

	/// Number of retries performed for applicable frames
	size_t NumRetry;

	/// The maximum size of received application layer fragments
	size_t FragSize;

};

}

#endif
