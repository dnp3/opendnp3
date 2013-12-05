#ifndef _PHYSICAL_LAYER_FACTORY_H_
#define _PHYSICAL_LAYER_FACTORY_H_


#include "SerialTypes.h"
#include "Exception.h"
#include "PhysicalLayerFunctors.h"
#include <map>

namespace opendnp3
{

class PhysicalLayerWrapperFactory
{
public:

	static IPhysicalLayerAsyncFactory GetSerialAsync(SerialSettings s);
	static IPhysicalLayerAsyncFactory GetTCPClientAsync(std::string aAddress, uint16_t aPort);
	static IPhysicalLayerAsyncFactory GetTCPServerAsync(std::string aEndpoint, uint16_t aPort);

	//normal factory functions
	static IPhysicalLayerWrapper* FGetSerialAsync(SerialSettings s, boost::asio::io_service* apSrv, openpal::Logger& arLogger);
	static IPhysicalLayerAsync* FGetTCPClientAsync(std::string aAddress, uint16_t aPort, boost::asio::io_service* apSrv, openpal::Logger& arLogger);
	static IPhysicalLayerAsync* FGetTCPServerAsync(std::string aEndpoint, uint16_t aPort, boost::asio::io_service* apSrv, openpal::Logger& arLogger);
};
}

#endif
