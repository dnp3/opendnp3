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

	SOEHandlerAdapter(DNP3::Interface::ISOEHandler^ proxy);

	virtual void Start() override final;
	virtual void End() override final;

	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) override final;

	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) override final;

private:

	template <class T>
	void DispatchStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<T, uint16_t>>& meas)
	{
		meas.foreach([this](const opendnp3::IndexedValue<T, uint16_t>& pair)
		{
			proxy->LoadStatic(Conversions::ConvertMeas(pair.value), pair.index);
		});
	}

	template <class T>
	void DispatchEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<T, uint16_t>>& meas)
	{
		meas.foreach([this](const opendnp3::IndexedValue<T, uint16_t>& pair)
		{
			proxy->LoadEvent(Conversions::ConvertMeas(pair.value), pair.index);
		});
	}

	gcroot < DNP3::Interface::ISOEHandler^ > proxy;
};

}
}

#endif
