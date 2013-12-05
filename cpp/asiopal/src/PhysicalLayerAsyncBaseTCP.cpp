#include <asiopal/PhysicalLayerAsyncBaseTCP.h>

#include <string>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>


using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncBaseTCP::PhysicalLayerAsyncBaseTCP(Logger& arLogger, boost::asio::io_service* apIOService) :
	PhysicalLayerAsyncASIO(arLogger, apIOService),
	mSocket(*apIOService)
{
	//mSocket.set_option(ip::tcp::no_delay(true));
}

/* Implement the actions */

void PhysicalLayerAsyncBaseTCP::DoClose()
{
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseTCP::DoAsyncRead(WriteBuffer& arBuffer)
{
	uint8_t* pBuff = arBuffer;
	mSocket.async_read_some(buffer(arBuffer, arBuffer.Size()),
							mStrand.wrap([this, pBuff](const boost::system::error_code& code, size_t numRead){
								this->OnReadCallback(code, pBuff, numRead);
							}));
}

void PhysicalLayerAsyncBaseTCP::DoAsyncWrite(const ReadOnlyBuffer& arBuffer)
{
	async_write(mSocket, buffer(arBuffer, arBuffer.Size()),
	            mStrand.wrap([this](const boost::system::error_code& code, size_t numWritten){
					this->OnWriteCallback(code, numWritten);
				}));
}

void PhysicalLayerAsyncBaseTCP::DoOpenFailure()
{
	LOG_BLOCK(LogLevel::Debug, "Failed socket open, closing socket");
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseTCP::CloseSocket()
{
	boost::system::error_code ec;

	mSocket.close(ec);
	if(ec) LOG_BLOCK(LogLevel::Warning, "Error while closing socket: " << ec.message());
}

void PhysicalLayerAsyncBaseTCP::ShutdownSocket()
{
	boost::system::error_code ec;

	mSocket.shutdown(ip::tcp::socket::shutdown_both, ec);
	if(ec) LOG_BLOCK(LogLevel::Warning, "Error while shutting down socket: " << ec.message());
}

}

/* vim: set ts=4 sw=4: */
