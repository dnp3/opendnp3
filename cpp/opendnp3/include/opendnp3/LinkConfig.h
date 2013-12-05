#ifndef __LINK_CONFIG_H_
#define __LINK_CONFIG_H_

#include <openpal/TimeDuration.h>

namespace opendnp3
{

/**
	Configuration for the dnp3 link layer
*/
struct LinkConfig {
	LinkConfig(
	        bool aIsMaster,
	        bool aUseConfirms,
	        size_t aNumRetry,
	        uint16_t aLocalAddr,
	        uint16_t aRemoteAddr,
	        openpal::TimeDuration aTimeout) :

		IsMaster(aIsMaster),
		UseConfirms(aUseConfirms),
		NumRetry(aNumRetry),
		LocalAddr(aLocalAddr),
		RemoteAddr(aRemoteAddr),
		Timeout(aTimeout)
	{}

	LinkConfig(
	        bool aIsMaster,
	        bool aUseConfirms) :

		IsMaster(aIsMaster),
		UseConfirms(aUseConfirms),
		NumRetry(0),
		LocalAddr(aIsMaster ? 1 : 1024),
		RemoteAddr(aIsMaster ? 1024 : 1),
		Timeout(openpal::TimeDuration::Seconds(1))
	{}

	/// The master/slave bit set on all messages
	bool IsMaster;

	/// If true, the link layer will send data requesting confirmation
	bool UseConfirms;

	/// The number of retry attempts the link will attempt after the initial try
	size_t NumRetry;

	/// dnp3 address of the local device
	uint16_t LocalAddr;

	/// dnp3 address of the remote device
	uint16_t RemoteAddr;

	/// the response timeout in milliseconds for confirmed requests
	openpal::TimeDuration Timeout;

private:

	LinkConfig() {}
};

}

#endif

