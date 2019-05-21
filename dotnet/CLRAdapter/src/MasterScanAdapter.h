#ifndef OPENDNP3CLR_MASTERSCANADAPTER_H
#define OPENDNP3CLR_MASTERSCANADAPTER_H

#include <asiodnp3/IMasterScan.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private ref class MasterScanAdapter : IMasterScan
			{
			public:

				MasterScanAdapter(const std::shared_ptr<asiodnp3::IMasterScan>& scan) : 
						scan(new std::shared_ptr <asiodnp3::IMasterScan>(scan))
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
					(*this->scan)->Demand();
				}

			private:

				std::shared_ptr <asiodnp3::IMasterScan>* scan;
			};

		}
	}
}

#endif
