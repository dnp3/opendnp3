#include <asiopal/PhysicalLayerAsyncSerial.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <string>

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>

#include <asiopal/ASIOSerialHelpers.h>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncSerial::PhysicalLayerAsyncSerial(
        Logger aLogger,
        boost::asio::io_service* apIOService,
        const SerialSettings& arSettings) :

	PhysicalLayerAsyncASIO(aLogger, apIOService),
	mSettings(arSettings),
	mPort(*apIOService)
{

}

/* Implement the actions */

void PhysicalLayerAsyncSerial::DoOpen()
{
	boost::system::error_code ec;
	mPort.open(mSettings.mDevice, ec);

	//use post to simulate an async open operation
	if(!ec) Configure(mSettings, mPort, ec);

	mExecutor.Post(std::bind(&PhysicalLayerAsyncSerial::OnOpenCallback, this, ec));
}

void PhysicalLayerAsyncSerial::DoClose()
{
	boost::system::error_code ec;
	mPort.close(ec);
	if(ec) LOG_BLOCK(LogLevel::Warning, ec.message());
}

void PhysicalLayerAsyncSerial::DoOpenSuccess()
{
	LOG_BLOCK(LogLevel::Info, "Port successfully opened");
}

void PhysicalLayerAsyncSerial::DoAsyncRead(openpal::WriteBuffer& arBuffer)
{
	uint8_t* pBuff = arBuffer;
	mPort.async_read_some(buffer(arBuffer, arBuffer.Size()),
	                      mStrand.wrap([this, pBuff](const boost::system::error_code& error, size_t numRead) {
							  this->OnReadCallback(error, pBuff, numRead);
						  }));	
}

void PhysicalLayerAsyncSerial::DoAsyncWrite(const ReadOnlyBuffer& arBuffer)
{
	async_write(mPort, buffer(arBuffer, arBuffer.Size()),
	            mStrand.wrap(
	                    std::bind(&PhysicalLayerAsyncSerial::OnWriteCallback,
	                              this,
	                              std::placeholders::_1,
								  arBuffer.Size())
	            ));
}

}

/* vim: set ts=4 sw=4: */

