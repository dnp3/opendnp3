
#include "Stdafx.h"
#include "OutstationApplicationAdapter.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			OutstationApplicationAdapter::OutstationApplicationAdapter(Automatak::DNP3::Interface::IOutstationApplication^ proxy_) :
				proxy(proxy_)
			{

			}

			bool OutstationApplicationAdapter::WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp)
			{
				return proxy->WriteAbsoluteTime(timestamp.msSinceEpoch);
			}

			bool OutstationApplicationAdapter::SupportsWriteAbsoluteTime()
			{
				return proxy->SupportsWriteAbsoluteTime;
			}

			opendnp3::ApplicationIIN OutstationApplicationAdapter::GetApplicationIIN() const
			{
				ApplicationIIN indications = proxy->ApplicationIndications;

				opendnp3::ApplicationIIN iin;
				iin.configCorrupt = indications.configCorrupt;
				iin.deviceTrouble = indications.deviceTrouble;
				iin.localControl = indications.localControl;
				iin.needTime = indications.needTime;
				return iin;
			}

			opendnp3::RestartMode OutstationApplicationAdapter::ColdRestartSupport() const
			{
				return (opendnp3::RestartMode) proxy->ColdRestartSupport;
			}

			opendnp3::RestartMode OutstationApplicationAdapter::WarmRestartSupport() const
			{
				return (opendnp3::RestartMode) proxy->WarmRestartSupport;
			}

			uint16_t OutstationApplicationAdapter::ColdRestart()
			{
				return proxy->ColdRestart();
			}

			uint16_t OutstationApplicationAdapter::WarmRestart()
			{
				return proxy->WarmRestart();
			}

		}
	}
}
