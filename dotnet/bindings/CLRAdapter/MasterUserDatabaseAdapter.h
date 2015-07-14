

#ifndef __MASTER_USER_DATABASE_ADAPTER_H_
#define __MASTER_USER_DATABASEADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <openpal/container/Buffer.h>
#include <secauth/master/IMasterUserDatabase.h>

#include <memory>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class MasterUserDatabaseAdapter : public secauth::IMasterUserDatabase
			{
			public:

				MasterUserDatabaseAdapter(Automatak::DNP3::Interface::IMasterUser^ proxy);
				
				virtual void EnumerateUsers(const std::function<void(const opendnp3::User)>& fun) const sealed;

				virtual bool GetUpdateKey(const opendnp3::User& user, opendnp3::UpdateKeyMode& type, openpal::ReadBufferView& key) const sealed;

				virtual bool UserExists(const opendnp3::User& user) const sealed;

			private:

				std::unique_ptr<openpal::Buffer> keyBuffer;

				gcroot < Automatak::DNP3::Interface::IMasterUser^ > proxy;
			};


		}
	}
}

#endif
