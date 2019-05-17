#ifndef OPENDNP3CLR_SESSION_ACCEPTOR_ADAPTER_H
#define	OPENDNP3CLR_SESSION_ACCEPTOR_ADAPTER_H

#include <asiodnp3/ISessionAcceptor.h>

#include <vcclr.h>

using namespace System;
using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private ref class SessionAcceptorAdapter sealed : public ISessionAcceptor
			{
			public:

				SessionAcceptorAdapter(asiodnp3::ISessionAcceptor& proxy);
				
				virtual IMasterSession^ AcceptSession(
					System::String^ loggerid,
					ISOEHandler^ SOEHandler,
					IMasterApplication^ application,
					MasterStackConfig^ config
				);
				
			private:

				asiodnp3::ISessionAcceptor* proxy;
			};

		}
	}
}

#endif
