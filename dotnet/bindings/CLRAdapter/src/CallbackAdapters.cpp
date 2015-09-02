

#include "CallbackAdapters.h"

#include "Conversions.h"

#include <vcclr.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{						
																
			opendnp3::CommandCallbackT CallbackAdapters::Get(TaskCompletionSource<CommandTaskResult^>^ tcs)
			{
				gcroot<TaskCompletionSource<CommandTaskResult^>^> handle(tcs);

				return [handle](const opendnp3::ICommandTaskResult& res) -> void
				{
					auto result = Conversions::ConvertCommandTaskResult(res);
					handle->SetResult(result);
				};
			}

			secauth::BeginUpdateKeyChangeCallbackT CallbackAdapters::Get(TaskCompletionSource<BeginUpdateKeyChangeResult^>^ tcs)
			{
				gcroot<TaskCompletionSource<BeginUpdateKeyChangeResult^>^> handle(tcs);

				return [handle](const secauth::BeginUpdateKeyChangeResult& rsp) -> void
				{
					BeginUpdateKeyChangeResult^ result = gcnew BeginUpdateKeyChangeResult(
						(TaskCompletion)  rsp.result,
						User::FromNumber(rsp.user.GetId()),
						rsp.keyChangeSequenceNum,
						Conversions::Convert(rsp.masterChallengeData.ToRSlice()),
						Conversions::Convert(rsp.outstationChallengeData.ToRSlice())						
					);


					handle->SetResult(result);
				};
			}

				

		}
	}
}


