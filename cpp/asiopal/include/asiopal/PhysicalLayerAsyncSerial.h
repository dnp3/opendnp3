#ifndef __PHYSICAL_LAYER_ASYNC_SERIAL_H_
#define __PHYSICAL_LAYER_ASYNC_SERIAL_H_

#include "PhysicalLayerAsyncASIO.h"

#include <openpal/Location.h>
#include <asiopal/SerialTypes.h>

#include <boost/asio/serial_port.hpp>

#include <memory>

namespace asiopal
{

/** Serial implementation of PhysicalLayerAsyncASIO
*/
class DLL_LOCAL PhysicalLayerAsyncSerial : public PhysicalLayerAsyncASIO
{
public:
	PhysicalLayerAsyncSerial(openpal::Logger, boost::asio::io_service* apIOService, const SerialSettings& arSettings);

	/* Implement the shared client/server actions */
	void DoClose();
	void DoOpenSuccess();
	void DoAsyncRead(openpal::WriteBuffer&);
	void DoAsyncWrite(const openpal::ReadOnlyBuffer&);

	void DoOpen();

protected:

	SerialSettings mSettings;
	boost::asio::serial_port mPort;
};
}

#endif
