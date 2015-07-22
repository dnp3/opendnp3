#ifndef __COMMAND_CALLBACK_ADAPTER_H_
#define __COMMAND_CALLBACK_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <openpal/util/Uncopyable.h>
#include <opendnp3/master/ICommandProcessor.h>

#include "Conversions.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			class CommandCallbackAdapter : public opendnp3::ICommandCallback, openpal::Uncopyable
			{
			public:
				CommandCallbackAdapter(Future<CommandResponse>^ future, bool autoDelete_) : autoDelete(autoDelete_), root(future)
				{}

				virtual void OnStart() sealed
				{
				
				}

				virtual void OnComplete(const opendnp3::CommandResponse& response) sealed
				{
					auto result = Conversions::ConvertCommandResponse(response);

					root->Set(result);
					if (autoDelete)
					{
						delete this;
					}
				}

				bool autoDelete;
				gcroot < Future<CommandResponse> ^ > root;
			};			

		}
	}
}

#endif
