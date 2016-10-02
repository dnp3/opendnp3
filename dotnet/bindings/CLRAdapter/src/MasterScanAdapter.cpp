

#include "MasterScanAdapter.h"

using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			MasterScanAdapter::MasterScanAdapter(const opendnp3::MasterScan& scan) : 
				scan(new opendnp3::MasterScan(scan))
			{
				
			}

			MasterScanAdapter::~MasterScanAdapter()
			{
				delete scan;				
			}			

			void MasterScanAdapter::Demand()
			{
				scan->Demand();
			}

		}
	}
}
