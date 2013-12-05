#include "WrappedTcpPipe.h"

namespace opendnp3
{

WrappedTcpPipe::WrappedTcpPipe(openpal::Logger& arLogger, boost::asio::io_service* apService, uint16_t aPort) :
	clientTcp(arLogger.GetSubLogger("clientTcp"), apService, "127.0.0.1", aPort),
	serverTcp(arLogger.GetSubLogger("serverTcp"), apService, "127.0.0.1", aPort),
	client(arLogger.GetSubLogger("clientWrapper"), &clientTcp),
	server(arLogger.GetSubLogger("serverWrapper"), &serverTcp)
{

}




}

