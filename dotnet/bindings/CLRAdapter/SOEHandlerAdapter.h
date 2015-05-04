#ifndef __SOE_HANDLER_ADAPTER_H_
#define __SOE_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/ISOEHandler.h>
#include <vcclr.h>

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

				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Binary& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::DoubleBitBinary& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Analog& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Counter& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::FrozenCounter& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::BinaryOutputStatus& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::AnalogOutputStatus& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::OctetString& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::TimeAndInterval& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::BinaryCommandEvent& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::AnalogCommandEvent& meas, uint16_t index) override final;
				virtual void OnValue(const opendnp3::HeaderInfo& info, const opendnp3::SecurityStat& meas, uint16_t index) override final;

			private:

				static HeaderInfo^ GetInfo(const opendnp3::HeaderInfo& info);

				gcroot < Automatak::DNP3::Interface::ISOEHandler^ > proxy;
			};

		}
	}
}

#endif
