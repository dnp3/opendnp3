#ifndef OPENDNP3CLR_MASTER_SESSION_ADAPTER_H
#define	OPENDNP3CLR_MASTER_SESSION_ADAPTER_H

#include <asiodnp3/IMasterSession.h>

#include "MasterOperationsAdapter.h"

#include <memory>

using namespace Automatak::DNP3::Interface;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private ref class MasterSessionAdapter sealed : IMasterSession, MasterOperationsAdapter
			{
			public:

				MasterSessionAdapter(std::shared_ptr<asiodnp3::IMasterSession> proxy);					
				
				MasterSessionAdapter::~MasterSessionAdapter()
				{
					this->!MasterSessionAdapter();
				}

				MasterSessionAdapter::!MasterSessionAdapter()
				{
					delete proxy;
				}

				/// --- implement IMasterSession ----				
				virtual void BeginShutdown();

				virtual Interface::IStackStatistics^ GetStackStatistics();

			private:
				
				std::shared_ptr<asiodnp3::IMasterSession>* proxy;
			};

		}
	}
}

#endif
