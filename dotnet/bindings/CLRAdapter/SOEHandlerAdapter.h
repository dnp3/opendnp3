#ifndef __SOE_HANDLER_ADAPTER_H_
#define __SOE_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/ISOEHandler.h>
#include <vcclr.h>

#include "EnumerableConversions.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class SOEHandlerAdapter : public opendnp3::ISOEHandler
			{
			public:

				SOEHandlerAdapter(Automatak::DNP3::Interface::ISOEHandler^ proxy);

				virtual void Start() override final;
				virtual void End() override final;

				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::TimeAndInterval, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryCommandEvent, uint16_t>>& meas) override final;
				virtual void OnReceiveHeader(const opendnp3::HeaderInfo& info, const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogCommandEvent, uint16_t>>& meas) override final;

			private:

				static HeaderInfo^ GetInfo(const opendnp3::HeaderInfo& info);

				gcroot < Automatak::DNP3::Interface::ISOEHandler^ > proxy;
			};

		}
	}
}

#endif
