#ifndef OPENDNP3CLR_TASK_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_TASK_CALLBACK_ADAPTER_H

#include "Conversions.h"

#include <openpal/util/Uncopyable.h>
#include <opendnp3/master/ITaskCallback.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			class TaskCallbackAdapter : public opendnp3::ITaskCallback, openpal::Uncopyable
			{
			public:

				static opendnp3::ITaskCallback* Create(Automatak::DNP3::Interface::ITaskCallback^ proxy)
				{
					return new TaskCallbackAdapter(proxy);
				}

				virtual void OnStart() sealed
				{
					root->OnStart();
				}

				virtual void OnComplete(opendnp3::TaskCompletion result) sealed
				{
					root->OnComplete((TaskCompletion) result);
				}

				virtual void OnDestroyed() sealed
				{
					root->OnDestroyed();
					delete this;
				}

			private:

				TaskCallbackAdapter();

				TaskCallbackAdapter(Automatak::DNP3::Interface::ITaskCallback^ proxy) : root(proxy)
				{}
				
				gcroot < Automatak::DNP3::Interface::ITaskCallback^ > root;
			};
		
		}
	}
}

#endif
