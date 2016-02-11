#include "SessionAcceptorAdapter.h"

#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "MasterApplicationAdapter.h"
#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

SessionAcceptorAdapter::SessionAcceptorAdapter(asiodnp3::ISessionAcceptor& proxy) : m_proxy(&proxy)
{}

IMasterSession^ SessionAcceptorAdapter::AcceptSession(
	System::String^ loggerid,
	ISOEHandler^ SOEHandler,
	IMasterApplication^ application,
	MasterStackConfig^ config
	)
{
	auto id = Conversions::ConvertString(loggerid);
	auto mconfig = Conversions::ConvertConfig(config);


	auto handler = new SOEHandlerAdapter(SOEHandler);
	auto app = new MasterApplicationAdapter<opendnp3::IMasterApplication>(application);
	
	auto session = m_proxy->AcceptSession(id, *handler, *app, mconfig);


	return gcnew MasterSessionAdapter(session);
}

}}} 