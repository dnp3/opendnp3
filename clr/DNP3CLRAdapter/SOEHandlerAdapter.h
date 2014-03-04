#ifndef __SOE_HANDLER_ADAPTER_H_
#define __SOE_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/ISOEHandler.h>
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
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) {}
	virtual void Load(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) {}

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
