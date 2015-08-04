#ifndef __MASTER_ADAPTER_SA_H_
#define __MASTER_ADAPTER_SA_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include "MasterAdapter.h"

#include "SAConversions.h"
#include "MasterConversions.h"

#include "CallbackAdapters.h"

#include <asiodnp3/IMasterSA.h>
#include <openpal/container/Buffer.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{		
			private ref class MasterAdapterStandard : public MasterAdapter, public IMaster
			{
			public:

				MasterAdapterStandard(asiodnp3::IMaster* apMaster) : MasterAdapter(apMaster)
				{}
		
			};

			private ref class MasterAdapterSA : public MasterAdapter, public IMasterSA
			{
			public:

				MasterAdapterSA(asiodnp3::IMasterSA* apMaster) : MasterAdapter(apMaster), pMasterSA(apMaster)
				{}	

				virtual void AddUser(Automatak::DNP3::Interface::User^ user, Automatak::DNP3::Interface::UpdateKey^ key) sealed
				{
					pMasterSA->AddUser(opendnp3::User(user->Number), MasterConversions::Convert(key));
				}

				virtual Task<TaskCompletion>^ ChangeUserStatus(UserStatusChange^ statusChange, TaskConfig^ config) sealed
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMasterSA->ChangeUserStatus(SAConversions::Convert(statusChange), MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				virtual Task<BeginUpdateKeyChangeResult^>^ BeginUpdateKeyChange(System::String^ username, TaskConfig^ config) sealed
				{
					auto configNative = MasterConversions::Convert(config);
					auto usernameNative = Conversions::ConvertString(username);
					
					auto tcs = gcnew TaskCompletionSource<BeginUpdateKeyChangeResult^>();
					
					pMasterSA->BeginUpdateKeyChange(usernameNative, configNative, CallbackAdapters::Get(tcs));
					
					return tcs->Task;
				}

				virtual Task<TaskCompletion>^ FinishUpdateKeyChange(FinishUpdateKeyChangeArgs^ args, TaskConfig^ config) sealed
				{
					auto configNative = MasterConversions::Convert(config);
					auto keyChangeArgsNative = MasterConversions::Convert(args);

					auto proxy = gcnew TaskCompletionProxy(config->callback);

					pMasterSA->FinishUpdateKeyChange(keyChangeArgsNative, MasterConversions::Convert(config, proxy));

					return proxy->CompletionTask;
				}
				
			private:
				
				asiodnp3::IMasterSA* pMasterSA;
			};

		}
	}
}
#endif
