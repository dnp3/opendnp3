#ifndef __MASTER_SCAN_ADAPTER_H_
#define __MASTER_SCAN_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/MasterScan.h>
#include <vcclr.h>


using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			class ScanListenerAdapter;

			private ref class MasterScanAdapter : IMasterScan
			{
			public:

				MasterScanAdapter(const opendnp3::MasterScan& scan);

				~MasterScanAdapter();

				virtual void AddScanCallback(System::Action<PollState>^ callback);

				void OnStateChane(Automatak::DNP3::Interface::PollState state);

				virtual void Demand();

			private:

				opendnp3::MasterScan* pScan;
				ScanListenerAdapter* pAdapter;

				List<System::Action<PollState>^>^ listeners;

			};

		}
	}
}

#endif
