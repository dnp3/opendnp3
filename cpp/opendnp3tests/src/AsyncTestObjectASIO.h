#ifndef __ASYNC_TEST_OBJECT_ASIO_H_
#define __ASYNC_TEST_OBJECT_ASIO_H_

#include "AsyncTestObject.h"

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace opendnp3
{

class AsyncTestObjectASIO : public AsyncTestObject
{
public:
	AsyncTestObjectASIO();
	AsyncTestObjectASIO(boost::asio::io_service*);
	virtual ~AsyncTestObjectASIO();

	boost::asio::io_service* GetService() {
		return mpTestObjectService;
	}

	static void Next(boost::asio::io_service* apSrv, openpal::TimeDuration aSleep);

private:
	boost::asio::io_service* mpTestObjectService;
	bool mOwner;
	void Next();

};

}

#endif
