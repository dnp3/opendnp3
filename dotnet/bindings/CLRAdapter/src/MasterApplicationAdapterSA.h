

#ifndef __MASTER_APPLICATION_ADAPTER_SA_H_
#define __MASTER_APPLICATION_ADAPTER_SA_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <secauth/master/IMasterApplicationSA.h>

#include "MasterApplicationAdapter.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			
			private class MasterApplicationAdapterSA : public MasterApplicationAdapter<secauth::IMasterApplicationSA>
			{
			public:

				MasterApplicationAdapterSA(Automatak::DNP3::Interface::IMasterApplicationSA^ proxy);
								
			private:				

				gcroot < Automatak::DNP3::Interface::IMasterApplicationSA^ > saproxy;
			};


		}
	}
}

#endif
