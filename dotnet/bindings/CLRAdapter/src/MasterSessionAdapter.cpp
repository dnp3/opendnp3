#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {


MasterSessionAdapter::MasterSessionAdapter(std::shared_ptr<asiodnp3::IMasterSession> proxy) :
	MasterOperationsAdapter(proxy.get()),
	m_proxy(new std::shared_ptr<asiodnp3::IMasterSession>(proxy))
{

}

MasterSessionAdapter::~MasterSessionAdapter()
{
	this->!MasterSessionAdapter();
}

MasterSessionAdapter::!MasterSessionAdapter()
{
	delete m_proxy;
}

void MasterSessionAdapter::BeginShutdown()
{
	(*m_proxy)->BeginShutdown();
}

}}}


