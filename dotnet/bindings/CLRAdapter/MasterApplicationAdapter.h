

#ifndef __MASTER_APPLICATION_ADAPTER_H_
#define __MASTER_APPLICATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/master/IMasterApplication.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class MasterApplicationAdapter : public opendnp3::IMasterApplication
{
public:
	MasterApplicationAdapter(DNP3::Interface::IMasterApplication^ proxy);

	virtual openpal::UTCTimestamp Now() override final;

private:
	gcroot < DNP3::Interface::IMasterApplication^ > proxy;
};


}
}

#endif
