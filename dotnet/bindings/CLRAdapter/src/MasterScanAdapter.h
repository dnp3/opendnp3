#ifndef OPENDNP3NET_MASTERSCANADAPTER_H_
#define OPENDNP3NET_MASTERSCANADAPTER_H_

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

			class TaskStateListenerAdapter;

			private ref class MasterScanAdapter : IMasterScan
			{
				public:

				MasterScanAdapter(const opendnp3::MasterScan& scan);

				~MasterScanAdapter();

				virtual void Demand();

				private:

				opendnp3::MasterScan* scan;								
			};

		}
	}
}

#endif
