#ifndef __OUTSTATION_ADAPTER_H_
#define __OUTSTATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/outstation/IOutstation.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class OutstationAdapter : IOutstation
{
public:

	OutstationAdapter(opendnp3::IOutstation* apOutstation);

	virtual IMeasurementLoader^ GetLoader();

	virtual void SetNeedTimeIIN();

	virtual void Shutdown();

	virtual void Enable();

	virtual void Disable();

private:
	opendnp3::IOutstation* mpOutstation;
	IMeasurementLoader^ mLoaderAdapter;
};

}
}

#endif
