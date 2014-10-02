
#include "Stdafx.h"
#include "TaskStateAdapter.h"

#include "TaskStateListenerAdapter.h"

using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			TaskStateAdapter::TaskStateAdapter(const opendnp3::MasterScan& scan)
			{
				pScan = new opendnp3::MasterScan(scan);
				pAdapter = new TaskStateListenerAdapter(this);				
				listeners = gcnew List<System::Action<TaskState>^>();

				pScan->SetTaskCallback(pAdapter->GetNotifier());
			}

			TaskStateAdapter::~TaskStateAdapter()
			{
				delete pScan;
				delete pAdapter;
			}

			void TaskStateAdapter::AddScanCallback(System::Action<TaskState>^ callback)
			{
				listeners->Add(callback);
			}

			void TaskStateAdapter::OnStateChane(Automatak::DNP3::Interface::TaskState state)
			{
				for each(auto callback in listeners)
				{
					callback->Invoke(state);
				}
			}

			void TaskStateAdapter::Demand()
			{
				pScan->Demand();
			}

		}
	}
}
