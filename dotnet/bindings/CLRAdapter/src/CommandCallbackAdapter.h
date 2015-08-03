
#ifndef OPENDNP3CLR_COMMAND_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_COMMAND_CALLBACK_ADAPTER_H

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
			class CommandCallbackAdapter : public opendnp3::CommandCallbackT, openpal::Uncopyable
			{

				public:
				
				CommandCallbackAdapter() : root(gcnew TaskCompletionSource<CommandResponse>()) {}

				virtual void OnComplete(const opendnp3::CommandResponse& response) sealed
				{
					auto result = Conversions::ConvertCommandResponse(response);

					root->SetResult(result);

					delete this;
				}

				Task<CommandResponse>^ GetTask()
				{
					return root->Task;
				}

				private:
				
				gcroot < TaskCompletionSource<CommandResponse>^ > root;				
			};			

		}
	}
}

#endif
