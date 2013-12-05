#include <opendnp3/MasterScan.h>

#include <openpal/IExecutor.h>
#include "AsyncTaskBase.h"

namespace opendnp3
{
	MasterScan::MasterScan(openpal::IExecutor* apExecutor, AsyncTaskBase* apTask) : 
		mpExecutor(apExecutor),
		mpTask(apTask)
	{
	
	}

	void MasterScan::Demand()
	{
		mpExecutor->Post([this]() { mpTask->Demand(); });
	}

}
