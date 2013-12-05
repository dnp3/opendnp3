#ifndef __MASTER_SCAN_ADAPTER_H_
#define __MASTER_SCAN_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/MasterScan.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private ref class MasterScanAdapter : IMasterScan
{
public:

	MasterScanAdapter(const opendnp3::MasterScan& arScan)
	{
		mpScan = new opendnp3::MasterScan(arScan);
	}

	~MasterScanAdapter()
	{
		delete mpScan;
	}

	virtual void Demand()
	{
		mpScan->Demand();
	}

private:

	opendnp3::MasterScan* mpScan;
};

}
}

#endif
