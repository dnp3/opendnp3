#ifndef __OUTSTATION_ADAPTER_H_
#define __OUTSTATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IOutstation.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class OutstationAdapter : IOutstation
{
public:

	OutstationAdapter(asiodnp3::IOutstation* apOutstation);

	virtual DNP3::Interface::IDatabase^ GetDatabase();

	virtual void SetNeedTimeIIN();

	virtual void Shutdown();

	virtual void Enable();

	virtual void Disable();

	virtual IStackStatistics^ GetStackStatistics();

private:
	asiodnp3::IOutstation* pOutstation;
	DNP3::Interface::IDatabase^ databaseAdapter;
};

}
}

#endif
