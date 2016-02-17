#include "SessionAcceptorAdapter.h"

#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "MasterApplicationAdapter.h"
#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

SessionAcceptorAdapter::SessionAcceptorAdapter(UInt64 sessionid, MasterSessionCache^ cache, asiodnp3::ISessionAcceptor& proxy) : 
	m_sessionid(sessionid),
	m_cache(cache),
	m_proxy(&proxy)
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
	
	auto handler = std::shared_ptr<SOEHandlerAdapter>(new SOEHandlerAdapter(SOEHandler));
	
	auto app = std::shared_ptr<MasterApplicationAdapter<opendnp3::IMasterApplication>>(
		new MasterApplicationAdapter<opendnp3::IMasterApplication>(application)
	);
	
	auto session = m_proxy->AcceptSession(id, handler, app, mconfig);

	if (!session)
	{
		return nullptr;
	}	

	// return a safe proxy that looks things up in the cache by id
	return m_cache->Add(m_sessionid, gcnew MasterSessionAdapter(session.get()));
}

}}} 