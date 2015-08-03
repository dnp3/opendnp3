

#include "Stdafx.h"
#include "CallbackAdapters.h"

#include "Conversions.h"

#include <vcclr.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{						
																
			opendnp3::CommandCallbackT CallbackAdapters::Get(TaskCompletionSource<CommandResponse>^ tcs)
			{
				auto root = new gcroot<TaskCompletionSource<CommandResponse>^>(tcs);

				return [root](const opendnp3::CommandResponse& rsp) -> void 
				{
					auto result = Conversions::ConvertCommandResponse(rsp);
					(*root)->SetResult(result);
					delete root;
				};
			}

			secauth::BeginUpdateKeyChangeCallbackT CallbackAdapters::Get(TaskCompletionSource<BeginUpdateKeyChangeResult^>^ tcs)
			{
				auto root = new gcroot<TaskCompletionSource<BeginUpdateKeyChangeResult^>^>(tcs);

				return [root](const secauth::BeginUpdateKeyChangeResult& rsp) -> void
				{
					BeginUpdateKeyChangeResult^ result = gcnew BeginUpdateKeyChangeResult(
						(TaskCompletion)  rsp.result,
						User::FromNumber(rsp.user.GetId()),
						rsp.keyChangeSequenceNum,
						Conversions::Convert(rsp.masterChallengeData.ToReadOnly()),
						Conversions::Convert(rsp.outstationChallengeData.ToReadOnly())
					);


					(*root)->SetResult(result);

					delete root;
				};
			}

				

		}
	}
}


