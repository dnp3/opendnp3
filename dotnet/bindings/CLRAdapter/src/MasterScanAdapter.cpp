

#include "MasterScanAdapter.h"

using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			MasterScanAdapter::MasterScanAdapter(const opendnp3::MasterScan& scan)
			{
				pScan = new opendnp3::MasterScan(scan);								
			}

			MasterScanAdapter::~MasterScanAdapter()
			{
				delete pScan;				
			}			

			void MasterScanAdapter::Demand()
			{
				pScan->Demand();
			}

		}
	}
}
