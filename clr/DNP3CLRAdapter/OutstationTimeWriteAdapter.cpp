#include "OutstationTimeWriteAdapter.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
OutstationTimeWriteAdapter::OutstationTimeWriteAdapter(DNP3::Interface::ITimeWriteHandler ^ proxy) :
	mProxy(proxy)
{

}

void OutstationTimeWriteAdapter::WriteAbsoluteTime(openpal::UTCTimestamp aTimestamp)
{
	mProxy->WriteAbsoluteTime(aTimestamp.msSinceEpoch);
}

}
}


