#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IMaster.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class MasterAdapter : IMaster
{
public:

	MasterAdapter(asiodnp3::IMaster* apMaster);

	virtual ICommandProcessor^ GetCommandProcessor();

	virtual void Enable();

	virtual void Disable();

	virtual void Shutdown();

	virtual IMasterScan^ GetIntegrityScan();

	virtual IMasterScan^ AddClassScan(int aClassMask, System::TimeSpan period, System::TimeSpan taskRetryPeriod);

private:
	asiodnp3::IMaster* mpMaster;
	ICommandProcessor^ mCommandAdapter;
};

}
}

#endif
