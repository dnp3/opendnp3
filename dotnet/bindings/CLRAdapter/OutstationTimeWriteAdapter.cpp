#include "OutstationTimeWriteAdapter.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
OutstationTimeWriteAdapter::OutstationTimeWriteAdapter(DNP3::Interface::ITimeWriteHandler^ proxy_) :
	proxy(proxy_)
{

}

bool OutstationTimeWriteAdapter::WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp)
{
	return proxy->WriteAbsoluteTime(timestamp.msSinceEpoch);
}

}
}


