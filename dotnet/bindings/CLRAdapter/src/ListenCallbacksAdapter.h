#ifndef CLR_DNP3_LISTEN_CALLBACKS_ADAPTER_H
#define	CLR_DNP3_LISTEN_CALLBACKS_ADAPTER_H

using namespace System;
using namespace Automatak::DNP3::Interface;

#include <asiodnp3/IListenCallbacks.h>
#include <vcclr.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private class ListenCallbacksAdapter final : public asiodnp3::IListenCallbacks
			{
			public:

				ListenCallbacksAdapter(Interface::IListenCallbacks^ proxy);

				virtual bool AcceptConnection(const std::string& ipaddress) override;
				virtual openpal::TimeDuration GetFirstFrameTimeout() override;
				virtual void OnFirstFrame(const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor) override;
				virtual void OnSessionClose(std::shared_ptr<asiodnp3::IGPRSMaster> session) override;
				

			private:
				
				gcroot < Interface::IListenCallbacks^ > proxy;
			};

		}
	}
}

#endif
