#ifndef __OUTSTATION_LOADER_ADAPTER_H_
#define __OUTSTATION_LOADER_ADAPTER_H_

#include <opendnp3/outstation/IMeasurementLoader.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class OutstationLoaderAdapter : DNP3::Interface::IMeasurementLoader
{
public:

	OutstationLoaderAdapter(opendnp3::IMeasurementLoader* proxy);

	virtual void Start();
	virtual void Update(DNP3::Interface::Binary^ meas, System::UInt32 index);
	virtual void Update(DNP3::Interface::Analog^ meas, System::UInt32 index);
	virtual void Update(DNP3::Interface::Counter^ meas, System::UInt32 index);
	virtual void Update(DNP3::Interface::FrozenCounter^ meas, System::UInt32 index);
	virtual void Update(DNP3::Interface::BinaryOutputStatus^ meas, System::UInt32 index);
	virtual void Update(DNP3::Interface::AnalogOutputStatus^ meas, System::UInt32 index);
	virtual void End();

private:

	opendnp3::IMeasurementLoader* proxy;
};

}
}

#endif
