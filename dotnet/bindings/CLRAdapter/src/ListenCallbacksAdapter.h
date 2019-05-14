#ifndef OPENDNP3CLR_LISTEN_CALLBACKS_ADAPTER_H
#define	OPENDNP3CLR_LISTEN_CALLBACKS_ADAPTER_H

#include <asiodnp3/IListenCallbacks.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System;

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

				virtual bool AcceptConnection(uint64_t sessionid, const std::string& ipaddress) override;
				virtual bool AcceptCertificate(uint64_t sessionid, const asiodnp3::X509Info& info) override;
				virtual openpal::TimeDuration GetFirstFrameTimeout() override;
				virtual void OnFirstFrame(uint64_t sessionid, const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor) override;
				virtual void OnConnectionClose(uint64_t sessionid, const std::shared_ptr<asiodnp3::IMasterSession>& session) override;
				virtual void OnCertificateError(uint64_t sessionid, const asiodnp3::X509Info& info, int err) override;

			private:
				
				gcroot < Interface::IListenCallbacks^ > m_proxy;				
			};

		}
	}
}

#endif
