

#ifndef __MASTER_USER_ADAPTER_H_
#define __MASTER_USER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <openpal/container/Buffer.h>
#include <secauth/master/IMasterUser.h>

#include <memory>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class MasterUserAdapter : public secauth::IMasterUser
			{
			public:
				MasterUserAdapter(Automatak::DNP3::Interface::IMasterUser^ proxy);
				
				virtual opendnp3::UpdateKeyMode GetUpdateKey(openpal::ReadBufferView& key) sealed;

			private:

				std::unique_ptr<openpal::Buffer> keyBuffer;


				gcroot < Automatak::DNP3::Interface::IMasterUser^ > proxy;
			};


		}
	}
}

#endif
