#ifndef __PHYSICAL_LAYER_ASYNC_ASIO_H_
#define __PHYSICAL_LAYER_ASYNC_ASIO_H_

#include "PhysicalLayerAsyncBase.h"
#include "ASIOExecutor.h"

#include <openpal/Location.h>

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace asiopal
{

// This is the base class for the new async physical layers. It assumes that all of the functions
// are called from a single thread.

class DLL_LOCAL PhysicalLayerAsyncASIO : public PhysicalLayerAsyncBase
{
public:
	PhysicalLayerAsyncASIO(openpal::Logger& arLogger, boost::asio::io_service* apService) :
		PhysicalLayerAsyncBase(arLogger),
		mStrand(*apService),
		mExecutor(&mStrand)
	{}

	virtual ~PhysicalLayerAsyncASIO() {}

	openpal::IExecutor* GetExecutor() {
		return &mExecutor;
	}

protected:
	boost::asio::strand mStrand;
	ASIOExecutor mExecutor;
};

}
#endif
