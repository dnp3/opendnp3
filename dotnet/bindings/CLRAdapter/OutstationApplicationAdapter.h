

#ifndef __OUTSTATION_APPLICATION_ADAPTER_H_
#define __OUTSTATION_APPLICATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/outstation/IOutstationApplication.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class OutstationApplicationAdapter : public opendnp3::IOutstationApplication
{
public:
	OutstationApplicationAdapter(DNP3::Interface::IOutstationApplication^ proxy);

	virtual bool WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp) override final;

	virtual bool SupportsWriteAbsoluteTime() override final;
	
	virtual opendnp3::ApplicationIIN GetApplicationIIN() const override final;
	
	virtual opendnp3::RestartMode ColdRestartSupport() const override final;
	
	virtual opendnp3::RestartMode WarmRestartSupport() const override final;
	
	virtual uint16_t ColdRestart();
	
	virtual uint16_t WarmRestart();

private:
	gcroot < DNP3::Interface::IOutstationApplication^ > proxy;
};


}
}

#endif
