#ifndef OPENDNP3NET_TASKSTATELISTENERADAPTER_H_
#define OPENDNP3NET_TASKSTATELISTENERADAPTER_H_


using namespace System::Collections::ObjectModel;


#include <vcclr.h>

#include "TaskStateAdapter.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class TaskStateListenerAdapter
			{
			public:

				TaskStateListenerAdapter(TaskStateAdapter^ adapter) : root(adapter)
				{}

				std::function<void(opendnp3::TaskState)> GetNotifier()
				{
					return [this](opendnp3::TaskState ts) { this->OnStateChange(ts); };
				}

			private:

				void OnStateChange(opendnp3::TaskState state)
				{
					root->OnStateChane((TaskState)state);
				}

				gcroot<TaskStateAdapter^> root;
			};

		}
	}
}

#endif
