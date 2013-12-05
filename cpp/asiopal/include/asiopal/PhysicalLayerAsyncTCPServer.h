#ifndef __PHYSICAL_LAYER_ASYNC_TCP_SERVER_H_
#define __PHYSICAL_LAYER_ASYNC_TCP_SERVER_H_

#include "PhysicalLayerAsyncBaseTCP.h"

#include <openpal/Location.h>

#include <boost/asio/ip/tcp.hpp>

namespace asiopal
{

class DLL_LOCAL PhysicalLayerAsyncTCPServer : public PhysicalLayerAsyncBaseTCP
{
public:
	PhysicalLayerAsyncTCPServer(
		openpal::Logger, 
		boost::asio::io_service* apIOService, 
		const std::string& arEndpoint,
		uint16_t aPort,
		std::function<void (boost::asio::ip::tcp::socket&)> aConfigure = [](boost::asio::ip::tcp::socket&){});

	/* Implement the remainging actions */
	void DoOpen();
	void DoOpeningClose(); //override this to cancel the acceptor instead of the socket
	void DoOpenSuccess();
	void DoOpenCallback();

private:

	void CloseAcceptor();

	boost::asio::ip::tcp::endpoint mLocalEndpoint;
	boost::asio::ip::tcp::endpoint mRemoteEndpoint;
	boost::asio::ip::tcp::acceptor mAcceptor;
	std::function<void (boost::asio::ip::tcp::socket&)> mConfigure;
};
}

#endif
