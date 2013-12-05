#ifndef __POSTING_MEASUREMENT_HANDLER_H_
#define	__POSTING_MEASUREMENT_HANDLER_H_

#include <opendnp3/MeasurementUpdate.h>
#include <opendnp3/IMeasurementHandler.h>

namespace openpal
{
	class IExecutor;
}

namespace opendnp3
{

class PostingMeasurementHandler
{
public:

	PostingMeasurementHandler(IMeasurementHandler* apProxy, openpal::IExecutor* apExecutor);
	
	const std::function<void (MeasurementUpdate&)> Load;
	
private:
	IMeasurementHandler* mpProxy;
	openpal::IExecutor* mpExecutor;
};

}

#endif
