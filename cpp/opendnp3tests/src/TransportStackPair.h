#ifndef __TRANSPORT_STACK_PAIR_H_
#define __TRANSPORT_STACK_PAIR_H_

namespace boost
{
namespace asio
{
class io_service;
}
}


#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include <openpal/IExecutor.h>

#include "TransportIntegrationStack.h"

namespace opendnp3
{

class TransportStackPair
{
public:
	TransportStackPair(
	        LinkConfig aClientCfg,
	        LinkConfig aServerCfg,
	        openpal::Logger& arLogger,
	        boost::asio::io_service* apService,
	        boost::uint16_t aPort);

	void Start();

	//test helper functions
	bool BothLayersUp();

public:
	asiopal::PhysicalLayerAsyncTCPClient mClient;
	asiopal::PhysicalLayerAsyncTCPServer mServer;

	TransportIntegrationStack mClientStack;
	TransportIntegrationStack mServerStack;

};

}

#endif

