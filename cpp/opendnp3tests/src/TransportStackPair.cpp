#include "TransportStackPair.h"

#include <boost/asio.hpp>

namespace opendnp3
{

TransportStackPair::TransportStackPair(
        LinkConfig aClientCfg,
        LinkConfig aServerCfg,
        openpal::Logger& arLogger,
        boost::asio::io_service* apService,
        boost::uint16_t aPort) :

	mClient(arLogger.GetSubLogger("TCPClient"), apService, "127.0.0.1", aPort),
	mServer(arLogger.GetSubLogger("TCPServer"), apService, "127.0.0.1", aPort),
	mClientStack(arLogger.GetSubLogger("ClientStack"), &mClient, aClientCfg),
	mServerStack(arLogger.GetSubLogger("ServerStack"), &mServer, aServerCfg)
{

}

bool TransportStackPair::BothLayersUp()
{
	return mServerStack.mUpper.IsLowerLayerUp()
	       && mClientStack.mUpper.IsLowerLayerUp();
}

void TransportStackPair::Start()
{
	mServerStack.mRouter.Start();
	mClientStack.mRouter.Start();
}

}

