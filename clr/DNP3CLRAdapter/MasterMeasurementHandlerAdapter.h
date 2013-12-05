#ifndef __MASTER_MEASUREMENT_HANDLER_ADAPTER_H_
#define __MASTER_MEASUREMENT_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IMeasurementHandler.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class MasterMeasurementHandlerAdapter : public opendnp3::IMeasurementHandler
{
public:

	MasterMeasurementHandlerAdapter(DNP3::Interface::IMeasurementHandler^ proxy);

	void Load(const opendnp3::IMeasurementUpdate& arUpdate);

private:

	gcroot < DNP3::Interface::IMeasurementHandler ^ > proxy;	
};

private ref class MasterMeasurementHandlerWrapper
{
public:

	MasterMeasurementHandlerWrapper(DNP3::Interface::IMeasurementHandler^ proxy) :
		mpAdapter(new MasterMeasurementHandlerAdapter(proxy))
	{}

	~MasterMeasurementHandlerWrapper() {
		delete mpAdapter;
	}

	opendnp3::IMeasurementHandler* Get() {
		return mpAdapter;
	}

private:
	MasterMeasurementHandlerAdapter* mpAdapter;
};

}
}

#endif
