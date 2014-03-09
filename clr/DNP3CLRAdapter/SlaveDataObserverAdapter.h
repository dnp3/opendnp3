#ifndef __SLAVE_DATA_OBSERVER_ADAPTER_H_
#define __SLAVE_DATA_OBSERVER_ADAPTER_H_

#include <opendnp3/outstation/IDataObserver.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class SlaveDataObserverAdapter : DNP3::Interface::IDataObserver
{
public:

	SlaveDataObserverAdapter(opendnp3::IDataObserver* proxy);

	virtual void Start();
	virtual void Update(Binary^ meas, System::UInt32 index);
	virtual void Update(Analog^ meas, System::UInt32 index);
	virtual void Update(Counter^ meas, System::UInt32 index);
	virtual void Update(FrozenCounter^ meas, System::UInt32 index);
	virtual void Update(BinaryOutputStatus^ meas, System::UInt32 index);
	virtual void Update(AnalogOutputStatus^ meas, System::UInt32 index);
	virtual void End();

private:

	opendnp3::IDataObserver* proxy;
};

}
}

#endif
