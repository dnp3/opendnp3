#ifndef __MASTER_ADAPTER_SA_H_
#define __MASTER_ADAPTER_SA_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include "MasterAdapter.h"

#include "SAConversions.h"
#include "MasterConversions.h"

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
					openpal::Buffer buffer(key->key->Length);
					for (int i = 0; i < key->key->Length; ++i)
					{
						buffer[i] = key->key[i];
					}

 					pMasterSA->AddUser(opendnp3::User(user->Number), secauth::UpdateKey(buffer.ToReadOnly()));

					// TODO use a secure buffer class instead of having to manually zero
					for (int i = 0; i < key->key->Length; ++i)
					{
						buffer[i] = 0;
					}
				}

				virtual Task<TaskCompletion>^ ChangeUserStatus(UserStatusChange^ statusChange, TaskConfig^ config) sealed
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMasterSA->ChangeUserStatus(SAConversions::Convert(statusChange), MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				virtual Task<BeginUpdateKeyChangeResult^>^ BeginUpdateKeyChange(System::String^ username) sealed
				{
					// TODO
					return nullptr;
				}
				
			private:
				
				asiodnp3::IMasterSA* pMasterSA;
			};

		}
	}
}
#endif
