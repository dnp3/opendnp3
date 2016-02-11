#ifndef CLR_DNP3_MASTER_SESSION_ADAPTER_H
#define	CLR_DNP3_MASTER_SESSION_ADAPTER_H

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

using namespace Automatak::DNP3::Interface;

#include <asiodnp3/IGPRSMaster.h>
#include <memory>

#include "MasterOperationsAdapter.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class MasterSessionAdapter sealed : IMasterSession, MasterOperationsAdapter
			{
			public:

				MasterSessionAdapter(std::shared_ptr<asiodnp3::IGPRSMaster> proxy);		

				~MasterSessionAdapter();
				!MasterSessionAdapter();

				virtual void BeginShutdown();				

			private:
				
				std::shared_ptr<asiodnp3::IGPRSMaster>* m_master;
			};

		}
	}
}

#endif
