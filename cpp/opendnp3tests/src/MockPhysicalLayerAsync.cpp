#include "MockPhysicalLayerAsync.h"

#include "BufferHelpers.h"
#include <openpal/IExecutor.h>

#include <memory.h>
#include <functional>

using namespace boost::system;
using namespace openpal;

namespace opendnp3
{

MockPhysicalLayerAsync::MockPhysicalLayerAsync(openpal::Logger aLogger, openpal::IExecutor* apExecutor) :
	PhysicalLayerAsyncBase(aLogger),
	mpWriteBuff(nullptr),
	mNumToRead(0),
	mNumToWrite(0),
	mNumWrites(0),
	mNumOpen(0),
	mNumOpenSuccess(0),
	mNumOpenFailure(0),
	mNumClose(0),
	mNumOpeningClose(0),

	mIsAutoOpenSuccess(true),
	mpExecutor(apExecutor)
{

}

void MockPhysicalLayerAsync::DoOpeningClose()
{
	++mNumOpeningClose;
}

void MockPhysicalLayerAsync::DoOpen()
{
	++mNumOpen;
}

void MockPhysicalLayerAsync::DoClose()
{
	++mNumClose;
}

void MockPhysicalLayerAsync::SignalOpenSuccess()
{
	error_code ec(errc::success, get_generic_category());
	this->OnOpenCallback(ec);
}

void MockPhysicalLayerAsync::SignalOpenFailure()
{
	error_code ec(errc::permission_denied, get_generic_category());
	this->OnOpenCallback(ec);
}

void MockPhysicalLayerAsync::SignalSendSuccess()
{
	size_t num = mNumToWrite; mNumToWrite = 0;
	error_code ec(errc::success, get_generic_category());
	this->OnWriteCallback(ec, num);
}

void MockPhysicalLayerAsync::SignalSendFailure()
{
	mNumToWrite = 0;
	error_code ec(errc::permission_denied, get_generic_category());
	this->OnWriteCallback(ec, 0);
}

void MockPhysicalLayerAsync::SignalReadFailure()
{
	mNumToRead = 0;
	error_code ec(errc::permission_denied, get_generic_category());
	this->OnReadCallback(ec, mpWriteBuff, 0);
}

void MockPhysicalLayerAsync::TriggerRead(const std::string& arData)
{
	HexSequence hs(arData);
	assert(hs.Size() <= this->mNumToRead);
	memcpy(mpWriteBuff, hs, hs.Size());
	mNumToRead = 0;
	error_code ec(errc::success, get_generic_category());
	this->OnReadCallback(ec, mpWriteBuff, hs.Size());
}

void MockPhysicalLayerAsync::TriggerClose()
{
	error_code ec(errc::connection_aborted, get_generic_category());
	this->OnReadCallback(ec, mpWriteBuff, 0);
}


}
