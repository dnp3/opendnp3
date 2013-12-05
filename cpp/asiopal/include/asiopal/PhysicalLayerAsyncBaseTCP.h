#ifndef __PHYSICAL_LAYER_ASYNC_BASE_TCP_H_
#define __PHYSICAL_LAYER_ASYNC_BASE_TCP_H_

#include "PhysicalLayerAsyncASIO.h"

#include <openpal/Location.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

namespace asiopal
{

/**
Common socket object and some shared implementations for server/client.
*/
class DLL_LOCAL PhysicalLayerAsyncBaseTCP : public PhysicalLayerAsyncASIO
{
public:
	PhysicalLayerAsyncBaseTCP(openpal::Logger&, boost::asio::io_service* apIOService);

	virtual ~PhysicalLayerAsyncBaseTCP() {}

	/* Implement the shared client/server actions */
	void DoClose();
	void DoAsyncRead(openpal::WriteBuffer&);
	void DoAsyncWrite(const openpal::ReadOnlyBuffer&);
	void DoOpenFailure();

protected:

	boost::asio::ip::tcp::socket mSocket;
	void CloseSocket();

private:
	void ShutdownSocket();

};
}

#endif
