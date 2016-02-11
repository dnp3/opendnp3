#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {


MasterSessionAdapter::MasterSessionAdapter(std::shared_ptr<asiodnp3::IGPRSMaster> proxy) :
	MasterOperationsAdapter(proxy.get()),
	m_master(new std::shared_ptr<asiodnp3::IGPRSMaster>(proxy))
{

}

MasterSessionAdapter::~MasterSessionAdapter()
{
	this->!MasterSessionAdapter();
}

MasterSessionAdapter::!MasterSessionAdapter()
{
	delete m_master;
}

void MasterSessionAdapter::BeginShutdown()
{
	(*m_master)->BeginShutdown();
}

}}}


