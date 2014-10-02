#ifndef OPENDNP3NET_TASKSTATEADAPTER_H_
#define OPENDNP3NET_TASKSTATEADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/master/MasterScan.h>
#include <vcclr.h>


using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			class TaskStateListenerAdapter;

			private ref class TaskStateAdapter : IMasterScan
			{
			public:

				TaskStateAdapter(const opendnp3::MasterScan& scan);

				~TaskStateAdapter();

				virtual void AddScanCallback(System::Action<TaskState>^ callback);				

				virtual void Demand();

				void OnStateChane(Automatak::DNP3::Interface::TaskState state);

			private:

				opendnp3::MasterScan* pScan;
				TaskStateListenerAdapter* pAdapter;

				List<System::Action<TaskState>^>^ listeners;

			};

		}
	}
}

#endif
