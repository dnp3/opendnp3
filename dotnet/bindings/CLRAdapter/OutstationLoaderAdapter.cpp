#include "OutstationLoaderAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


OutstationLoaderAdapter::OutstationLoaderAdapter(opendnp3::IMeasurementLoader* proxy) :
	proxy(proxy)
{}

void OutstationLoaderAdapter::Start()
{
	opendnp3::Transaction::Start(proxy);
}

void OutstationLoaderAdapter::Update(Binary^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::Update(Analog^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::Update(Counter^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::Update(FrozenCounter^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::Update(BinaryOutputStatus^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::Update(AnalogOutputStatus^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationLoaderAdapter::End()
{
	opendnp3::Transaction::End(proxy);
}


}
}

