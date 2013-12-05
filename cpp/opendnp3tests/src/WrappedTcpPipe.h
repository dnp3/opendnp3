#ifndef __WRAPPED_TCP_PIPE_H_
#define __WRAPPED_TCP_PIPE_H_

#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include "PhysicalLayerWrapper.h"

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace opendnp3
{

class WrappedTcpPipe
{
public:
	WrappedTcpPipe(openpal::Logger&, boost::asio::io_service*, uint16_t aPort);

private:
	asiopal::PhysicalLayerAsyncTCPClient clientTcp;
	asiopal::PhysicalLayerAsyncTCPServer serverTcp;

public:
	PhysicalLayerWrapper client;
	PhysicalLayerWrapper server;
};

}

#endif
