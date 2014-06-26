#ifndef __SCAN_LISTENER_ADAPTER_H_
#define __SCAN_LISTENER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/IPollListener.h>
#include <vcclr.h>

#include "MasterScanAdapter.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class ScanListenerAdapter : public opendnp3::IPollListener
{
public:

	ScanListenerAdapter(MasterScanAdapter^ adapter) : root(adapter)
	{}

	
	virtual void OnStateChange(opendnp3::PollState state)
	{		
		root->OnStateChane((PollState)state);
	}

private:

	gcroot<MasterScanAdapter^> root;
};

}
}

#endif
