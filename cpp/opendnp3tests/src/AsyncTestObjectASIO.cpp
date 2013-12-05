#include "AsyncTestObjectASIO.h"
#include <boost/asio.hpp>

#include <openpal/Location.h>
#include <openpal/Exception.h>

#include <thread>
#include <chrono>

using namespace openpal;

namespace opendnp3
{

AsyncTestObjectASIO::AsyncTestObjectASIO() :
	mpTestObjectService(new boost::asio::io_service()),
	mOwner(true)
{}

AsyncTestObjectASIO::AsyncTestObjectASIO(boost::asio::io_service* apService) :
	mpTestObjectService(apService),
	mOwner(false)
{

}

AsyncTestObjectASIO::~AsyncTestObjectASIO()
{
	if(mOwner) delete mpTestObjectService;
}

void AsyncTestObjectASIO::Next()
{
	Next(this->GetService(), TimeDuration::Milliseconds(10));
}

void AsyncTestObjectASIO::Next(boost::asio::io_service* apSrv, openpal::TimeDuration aSleep)
{
	boost::system::error_code ec;
	size_t num = apSrv->poll_one(ec);
	if(ec) throw Exception(LOCATION, ec.message());
	if(num == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(aSleep.GetMilliseconds()));
	}
	apSrv->reset();
}



}
