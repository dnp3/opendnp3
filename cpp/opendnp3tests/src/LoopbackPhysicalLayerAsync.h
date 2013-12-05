#ifndef __LOOP_BACK_PHYSICAL_LAYER_ASYNC_H_
#define __LOOP_BACK_PHYSICAL_LAYER_ASYNC_H_

#include <asiopal/PhysicalLayerAsyncASIO.h>

#include <openpal/IHandlerAsync.h>

#include <queue>

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace opendnp3
{

// Provides a backend for testing physical layers
class LoopbackPhysicalLayerAsync : public asiopal::PhysicalLayerAsyncASIO
{
public:
	LoopbackPhysicalLayerAsync(openpal::Logger, boost::asio::io_service* apSrv);


private:

	void DoOpen();
	void DoClose();
	void DoOpenSuccess();
	void DoAsyncRead(openpal::WriteBuffer&);
	void DoAsyncWrite(const openpal::ReadOnlyBuffer&);


	void CheckForReadDispatch();

	std::deque<uint8_t> mWritten;

	openpal::WriteBuffer mBytesForReading;
};
}

#endif
