#include "WrappedTcpPipe.h"

#include <APL/Logger.h>

namespace apl
{

WrappedTcpPipe::WrappedTcpPipe(Logger* apLogger, boost::asio::io_service* apService, uint16_t aPort) :
	clientTcp(apLogger->GetSubLogger("clientTcp"), apService, "127.0.0.1", aPort),
	serverTcp(apLogger->GetSubLogger("serverTcp"), apService, "127.0.0.1", aPort),
	client(apLogger->GetSubLogger("clientWrapper"), &clientTcp),
	server(apLogger->GetSubLogger("serverWrapper"), &serverTcp)
{

}




}

