#include "SlaveDataObserverAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


SlaveDataObserverAdapter::SlaveDataObserverAdapter(opendnp3::IDataObserver* proxy) :
	proxy(proxy)
{}

void SlaveDataObserverAdapter::Start()
{
	opendnp3::Transaction::Start(proxy);
}

void SlaveDataObserverAdapter::Update(Binary^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(Analog^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(Counter^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(FrozenCounter^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(BinaryOutputStatus^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(AnalogOutputStatus^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::End()
{
	opendnp3::Transaction::End(proxy);
}


}
}

