
#include "Stdafx.h"
#include "OutstationDatabaseAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


OutstationDatabaseAdapter::OutstationDatabaseAdapter(opendnp3::IDatabase& proxy) :
	pProxy(&proxy)
{}

void OutstationDatabaseAdapter::Start()
{
	opendnp3::Transaction::Start(pProxy);
}

void OutstationDatabaseAdapter::Update(Binary^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::Update(Analog^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::Update(Counter^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::Update(FrozenCounter^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::Update(BinaryOutputStatus^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::Update(AnalogOutputStatus^ meas, System::UInt32 index)
{
	pProxy->Update(Conversions::ConvertMeas(meas), index);
}

void OutstationDatabaseAdapter::End()
{
	opendnp3::Transaction::End(pProxy);
}


}
}

