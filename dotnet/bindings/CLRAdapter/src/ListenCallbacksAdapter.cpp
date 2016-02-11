
#include "ListenCallbacksAdapter.h"

#include "Conversions.h"
#include "SessionAcceptorAdapter.h"
#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

	ListenCallbacksAdapter::ListenCallbacksAdapter(Interface::IListenCallbacks^ proxy) : m_proxy(proxy)
	{}

	bool ListenCallbacksAdapter::AcceptConnection(const std::string& ipaddress)
	{		
		return m_proxy->AcceptConnection(Conversions::ConvertString(ipaddress));
	}

	openpal::TimeDuration ListenCallbacksAdapter::GetFirstFrameTimeout()
	{
		return Conversions::ConvertTimespan(m_proxy->GetFirstFrameTimeout());
	}

	void ListenCallbacksAdapter::OnFirstFrame(const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor)
	{
		auto linkheader = Conversions::Convert(header);
		auto adapter = gcnew SessionAcceptorAdapter(acceptor);
		m_proxy->OnFirstFrame(linkheader, adapter);
	}

	void ListenCallbacksAdapter::OnSessionClose(std::shared_ptr<asiodnp3::IGPRSMaster> session)
	{
		m_proxy->OnSessionClose(gcnew MasterSessionAdapter(session));
	}

}}}


