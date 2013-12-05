#include <asiopal/PhysicalLayerAsyncTCPClient.h>

#include <boost/asio.hpp>
#include <functional>
#include <string>


#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>

using namespace boost;
using namespace boost::asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncTCPClient::PhysicalLayerAsyncTCPClient(
		Logger aLogger, 
		boost::asio::io_service* apIOService, 
		const std::string& arAddress, 
		uint16_t aPort, 
		std::function<void (boost::asio::ip::tcp::socket&)> aConfigure) :

	PhysicalLayerAsyncBaseTCP(aLogger, apIOService),
	mRemoteEndpoint(ip::tcp::v4(), aPort),
	mConfigure(aConfigure)
{
	mRemoteEndpoint.address( boost::asio::ip::address::from_string(arAddress) );
}

/* Implement the actions */
void PhysicalLayerAsyncTCPClient::DoOpen()
{
	mSocket.async_connect(mRemoteEndpoint,
							mStrand.wrap([this](const boost::system::error_code& code){
								this->OnOpenCallback(code);
							}));					   							
}

void PhysicalLayerAsyncTCPClient::DoOpeningClose()
{
	this->CloseSocket();
}

void PhysicalLayerAsyncTCPClient::DoOpenSuccess()
{
	LOG_BLOCK(LogLevel::Info, "Connected to: " << mRemoteEndpoint);
	mConfigure(mSocket);
}

}

/* vim: set ts=4 sw=4: */
