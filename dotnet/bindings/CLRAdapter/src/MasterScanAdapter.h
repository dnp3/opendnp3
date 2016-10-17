#ifndef OPENDNP3NET_MASTERSCANADAPTER_H_
#define OPENDNP3NET_MASTERSCANADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/MasterScan.h>
#include <vcclr.h>

using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace opendnp3
{
	struct 
}

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{			
			private ref class MasterScanAdapter : IMasterScan
			{
				public:

				MasterScanAdapter(const asiodnp3::MasterScan& scan) : scan(new asiodnp3::MasterScan(scan))
				{}

				~MasterScanAdapter()
				{
					this->!MasterScanAdapter();
				}
				
				!MasterScanAdapter()
				{
					delete scan;
				}

				virtual void Demand()
				{
					this->scan->Demand();
				}

				private:

				asiodnp3::MasterScan* scan;
			};

		}
	}
}

#endif
