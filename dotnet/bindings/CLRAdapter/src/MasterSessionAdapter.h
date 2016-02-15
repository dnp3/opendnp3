#ifndef CLR_DNP3_MASTER_SESSION_ADAPTER_H
#define	CLR_DNP3_MASTER_SESSION_ADAPTER_H

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

using namespace Automatak::DNP3::Interface;

#include <asiodnp3/IMasterSession.h>

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

				MasterSessionAdapter(asiodnp3::IMasterSession* proxy);						

				/// --- implement IMasterSession ----

				virtual bool IsSameSession(IMasterSession^ other);
				virtual void BeginShutdown();

			private:
				
				asiodnp3::IMasterSession* m_master;
			};

		}
	}
}

#endif
