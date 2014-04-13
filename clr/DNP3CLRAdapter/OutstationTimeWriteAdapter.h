#ifndef __OUTSTATION_TIME_WRITE_ADAPTER_H_
#define __OUTSTATION_TIME_WRITE_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/outstation/ITimeWriteHandler.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
//this object goes into the stack
private class OutstationTimeWriteAdapter : public opendnp3::ITimeWriteHandler
{
public:
	OutstationTimeWriteAdapter(DNP3::Interface::ITimeWriteHandler^ proxy);

	bool WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp);


private:
	gcroot < DNP3::Interface::ITimeWriteHandler^ > proxy;
};

private ref class OutstationTimeWriteWrapper
{
public:

	OutstationTimeWriteWrapper(DNP3::Interface::ITimeWriteHandler^ proxy) :
		mpAdapter(new OutstationTimeWriteAdapter(proxy))
	{}

	~OutstationTimeWriteWrapper()
	{
		delete mpAdapter;
	}

	opendnp3::ITimeWriteHandler* Get()
	{
		return mpAdapter;
	}

private:
	OutstationTimeWriteAdapter* mpAdapter;
};
}
}

#endif
