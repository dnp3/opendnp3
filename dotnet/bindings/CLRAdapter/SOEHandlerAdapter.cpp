
#include "Stdafx.h"
#include "SOEHandlerAdapter.h"
#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			SOEHandlerAdapter::SOEHandlerAdapter(Automatak::DNP3::Interface::ISOEHandler^ aProxy) : proxy(aProxy)
			{}

			void SOEHandlerAdapter::Start()
			{
				proxy->Start();
			}

			void SOEHandlerAdapter::End()
			{
				proxy->End();
			}

			HeaderInfo^ SOEHandlerAdapter::GetInfo(const opendnp3::HeaderInfo& info)
			{				
				return gcnew HeaderInfo((GroupVariation)info.gv, (QualifierCode)info.qualifier, (TimestampMode)info.tsmode, info.headerCount);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Binary& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::DoubleBitBinary& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Analog& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}
			
			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::Counter& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::FrozenCounter& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::BinaryOutputStatus& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::AnalogOutputStatus& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::OctetString& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::TimeAndInterval& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::BinaryCommandEvent& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::AnalogCommandEvent& meas, uint16_t index)
			{
				proxy->OnValue(GetInfo(info), Conversions::ConvertMeas(meas), index);
			}

			void SOEHandlerAdapter::OnValue(const opendnp3::HeaderInfo& info, const opendnp3::SecurityStat& meas, uint16_t index)
			{
				// TODO - create type and convert
			}
		}
	}
}
