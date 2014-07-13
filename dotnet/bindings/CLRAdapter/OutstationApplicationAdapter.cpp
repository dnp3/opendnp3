
#include "Stdafx.h"
#include "OutstationApplicationAdapter.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

OutstationApplicationAdapter::OutstationApplicationAdapter(DNP3::Interface::IOutstationApplication^ proxy_) :
	proxy(proxy_)
{

}

bool OutstationApplicationAdapter::WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp)
{
	return proxy->WriteAbsoluteTime(timestamp.msSinceEpoch);
}

bool OutstationApplicationAdapter::SupportsWriteAbsoluteTime()
{
	return proxy->SupportsWriteAbsoluteTime();
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

}
}


