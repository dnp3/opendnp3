#ifndef __SCAN_LISTENER_ADAPTER_H_
#define __SCAN_LISTENER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/IScanListener.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class ScanListenerAdapter : public opendnp3::IScanListener
{
public:

	ScanListenerAdapter(System::Action<ScanResult>^ callback) : myCallback(callback)
	{}

	virtual void OnScanUpdate(opendnp3::ScanResult result) final
	{
		ScanStatus status = (result.status == opendnp3::ScanStatus::SUCCESS) ? ScanStatus::SUCCESS : ScanStatus::FAILURE;
		myCallback->Invoke(ScanResult(status));
	}

private:

	gcroot<System::Action<ScanResult>^> myCallback;
};

}
}

#endif
