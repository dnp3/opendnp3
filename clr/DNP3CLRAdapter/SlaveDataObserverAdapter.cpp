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
	proxy->Start();
}

void SlaveDataObserverAdapter::Update(Binary ^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(Analog ^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(Counter ^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(ControlStatus ^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::Update(SetpointStatus ^ meas, System::UInt32 index)
{
	proxy->Update(Conversions::convertMeas(meas), index);
}

void SlaveDataObserverAdapter::End()
{
	proxy->End();
}


}
}

