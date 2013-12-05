#include "MasterMeasurementHandlerAdapter.h"


#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{

MasterMeasurementHandlerAdapter::MasterMeasurementHandlerAdapter(DNP3::Interface::IMeasurementHandler^ aProxy) : proxy(aProxy)
{}

void MasterMeasurementHandlerAdapter::Load(const opendnp3::IMeasurementUpdate& arUpdate)
{
	auto updates = gcnew MeasurementUpdate();
	for(auto v: arUpdate.BinaryUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	for(auto v: arUpdate.AnalogUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	for(auto v: arUpdate.CounterUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	for(auto v: arUpdate.ControlStatusUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	for(auto v: arUpdate.SetpointStatusUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	for(auto v: arUpdate.OctetStringUpdates()) updates->Load(Conversions::convertMeas(v.value), v.index);
	proxy->Load(updates);
}

}
}
