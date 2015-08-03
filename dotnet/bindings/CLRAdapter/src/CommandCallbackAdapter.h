#ifndef __COMMAND_CALLBACK_ADAPTER_H_
#define __COMMAND_CALLBACK_ADAPTER_H_

using namespace System::Collections::ObjectModel;
using namespace System::Threading::Tasks;

#include <vcclr.h>

#include <openpal/util/Uncopyable.h>


using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			template <class T, class U>
			class TaskCallbackAdapter : public opendnp3::IResultCallback<T>, openpal::Uncopyable
			{
			public:

				TaskCallbackAdapter(TaskCompletionSource<U>^ taskCompletionSource, System::Func<U^, T> conversion) :
					root(taskCompletionSource),
					conversion(conversion_)
				{
				
				}				

				virtual void OnComplete(const T& response) sealed
				{
					auto result = conversion->Invoke(response);

					root->SetResult(result);

					delete this;
				}
				
				gcroot < TaskCompletionSource<U>^ > root;
				gcroot < System::Func<U^, T> > conversion;
			};			

		}
	}
}

#endif
