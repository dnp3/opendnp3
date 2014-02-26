#ifndef __SOE_HANDLER_ADAPTER_H_
#define __SOE_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/ISOEHandler.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class SOEHandlerAdapter : public opendnp3::ISOEHandler
{
public:

	SOEHandlerAdapter(DNP3::Interface::IMeasurementHandler^ proxy);

	virtual void Start() final {}
	virtual void End() final {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::ControlStatus>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::SetpointStatus>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString>>& meas) {}

private:

	gcroot < DNP3::Interface::IMeasurementHandler ^ > proxy;	
};

private ref class MasterMeasurementHandlerWrapper
{
public:

	MasterMeasurementHandlerWrapper(DNP3::Interface::IMeasurementHandler^ proxy) :
		mpAdapter(new SOEHandlerAdapter(proxy))
	{}

	~MasterMeasurementHandlerWrapper() {
		delete mpAdapter;
	}

	opendnp3::ISOEHandler* Get() {
		return mpAdapter;
	}

private:
	SOEHandlerAdapter* mpAdapter;
};

}
}

#endif
