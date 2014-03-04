#ifndef __MASTER_SCAN_ADAPTER_H_
#define __MASTER_SCAN_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/MasterScan.h>
#include <vcclr.h>

#include "ScanListenerAdapter.h"

using namespace DNP3::Interface;
using namespace System::Collections::Generic;

namespace DNP3
{
namespace Adapter
{

template<typename T>
public value class Wrapper sealed
{
public:
	Wrapper(T* ptr) : pointer(ptr) {}

	T* pointer;
};

private ref class MasterScanAdapter : IMasterScan
{
public:

	MasterScanAdapter(const opendnp3::MasterScan& arScan)
	{
		mpScan = new opendnp3::MasterScan(arScan);
		listeners = gcnew List<Wrapper<ScanListenerAdapter>>();
	}

	~MasterScanAdapter()
	{
		delete mpScan;
		for each (Wrapper<ScanListenerAdapter> var in listeners)
		{
			delete var.pointer;
		}
	}	

	virtual void AddScanCallback(System::Action<ScanResult>^ callback)
	{
		auto pCallback = new ScanListenerAdapter(callback);
		mpScan->AddScanCallback(pCallback);
		Wrapper<ScanListenerAdapter> wrapper(pCallback);
		listeners->Add(wrapper);
	}

	virtual void Demand()
	{
		mpScan->Demand();
	}

private:
	
	List<Wrapper<ScanListenerAdapter>>^ listeners;
	opendnp3::MasterScan* mpScan;
};

}
}

#endif
