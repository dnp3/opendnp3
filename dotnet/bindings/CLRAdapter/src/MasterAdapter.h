#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include "MasterAdapterBase.h"

#include "MasterConversions.h"

#include "CallbackAdapters.h"

#include <openpal/container/Buffer.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{		
			private ref class MasterAdapter : public MasterAdapterBase, public IMaster
			{
			public:

				MasterAdapter(asiodnp3::IMaster* apMaster) : MasterAdapterBase(apMaster)
				{}
		
			};			

		}
	}
}
#endif
