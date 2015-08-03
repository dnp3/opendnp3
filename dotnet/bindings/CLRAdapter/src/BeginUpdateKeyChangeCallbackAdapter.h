
#ifndef OPENDNP3CLR_BEGIN_UPDATE_KEY_CHANGE_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_BEGIN_UPDATE_KEY_CHANGE_CALLBACK_ADAPTER_H

using namespace System::Collections::ObjectModel;
using namespace System::Threading::Tasks;

#include <vcclr.h>

#include <openpal/util/Uncopyable.h>

#include <secauth/master/IBeginUpdateKeyChangeHandler.h>

#include "Conversions.h"


using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{			
			class BeginUpdateKeyChangeCallbackAdapter : public secauth::IBeginUpdateKeyChangeHandler, openpal::Uncopyable
			{

				public:
				
				BeginUpdateKeyChangeCallbackAdapter() : root(gcnew TaskCompletionSource<BeginUpdateKeyChangeResult^>()) {}

				virtual void OnComplete(const secauth::BeginUpdateKeyChangeResult& response) sealed
				{
					auto result = gcnew BeginUpdateKeyChangeResult(
						(TaskCompletion) response.result,
						User::FromNumber(response.user.GetId()),
						response.keyChangeSequenceNum,
						Conversions::Convert(response.masterChallengeData.ToReadOnly()),
						Conversions::Convert(response.outstationChallengeData.ToReadOnly())
					);

					root->SetResult(result);

					delete this;
				}

				Task<BeginUpdateKeyChangeResult^>^ GetTask()
				{
					return root->Task;
				}

				private:
				
				gcroot < TaskCompletionSource<BeginUpdateKeyChangeResult^>^ > root;
			};			

		}
	}
}

#endif
