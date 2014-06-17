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

	virtual IStackStatistics^ GetStackStatistics();

	virtual void Enable();

	virtual void Disable();

	virtual void Shutdown();	

	virtual IMasterScan^ AddClassScan(int aClassMask, System::TimeSpan period);

	virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period);

private:
	asiodnp3::IMaster* pMaster;
	ICommandProcessor^ commandAdapter;
};

}
}

#endif
