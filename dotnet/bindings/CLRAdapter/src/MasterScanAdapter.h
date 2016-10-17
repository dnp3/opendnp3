#ifndef OPENDNP3NET_MASTERSCANADAPTER_H_
#define OPENDNP3NET_MASTERSCANADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/MasterScan.h>
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

				MasterScanAdapter(const asiodnp3::MasterScan& scan);

				~MasterScanAdapter();

				virtual void Demand();

				private:

				asiodnp3::MasterScan* scan;
			};

		}
	}
}

#endif
