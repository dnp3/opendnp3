#ifndef __OUTSTATION_DATABASE_ADAPTER_H_
#define __OUTSTATION_DATABASE_ADAPTER_H_

#include <opendnp3/outstation/IDatabase.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class OutstationDatabaseAdapter : DNP3::Interface::IDatabase
{
public:

	OutstationDatabaseAdapter(opendnp3::IDatabase& proxy);

	virtual void Start();
	virtual void Update(DNP3::Interface::Binary^ meas, System::UInt16 index);
	virtual void Update(DNP3::Interface::Analog^ meas, System::UInt16 index);
	virtual void Update(DNP3::Interface::Counter^ meas, System::UInt16 index);
	virtual void Update(DNP3::Interface::FrozenCounter^ meas, System::UInt16 index);
	virtual void Update(DNP3::Interface::BinaryOutputStatus^ meas, System::UInt16 index);
	virtual void Update(DNP3::Interface::AnalogOutputStatus^ meas, System::UInt16 index);
	virtual void End();

private:

	opendnp3::IDatabase* pProxy;
};

}
}

#endif
