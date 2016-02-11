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

				virtual bool IsSameSession(IMasterSession^ other)
				{
					if (other->GetType() != MasterSessionAdapter::typeid)
					{
						return false;
					}

					auto ref = (MasterSessionAdapter^)(other);

					// perform shared_ptr equality
					return ref->m_master == m_master;					
				}

				virtual void BeginShutdown();

			private:
				
				std::shared_ptr<asiodnp3::IGPRSMaster>* m_master;
			};

		}
	}
}

#endif
