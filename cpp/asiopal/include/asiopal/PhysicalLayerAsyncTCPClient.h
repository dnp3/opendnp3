#ifndef __PHYSICAL_LAYER_ASYNC_TCP_CLIENT_H_
#define __PHYSICAL_LAYER_ASYNC_TCP_CLIENT_H_

#include "PhysicalLayerAsyncBaseTCP.h"

#include <boost/asio/ip/tcp.hpp>

#include <openpal/Location.h>

namespace asiopal
{

class DLL_LOCAL PhysicalLayerAsyncTCPClient : public PhysicalLayerAsyncBaseTCP
{
public:
	PhysicalLayerAsyncTCPClient(
		openpal::Logger aLogger, 
		boost::asio::io_service* apIOService, 
		const std::string& arAddress, 
		uint16_t aPort, 
		std::function<void (boost::asio::ip::tcp::socket&)> aConfigure = [](boost::asio::ip::tcp::socket&){});

	/* Implement the remaining actions */
	void DoOpen();
	void DoOpeningClose(); //override this to just close the socket insead of shutting is down too
	void DoOpenSuccess();

private:
	boost::asio::ip::tcp::endpoint mRemoteEndpoint;
	std::function<void (boost::asio::ip::tcp::socket&)> mConfigure;
};

}

#endif
