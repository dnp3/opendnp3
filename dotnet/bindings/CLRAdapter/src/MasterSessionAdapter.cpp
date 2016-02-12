#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {


MasterSessionAdapter::MasterSessionAdapter(std::shared_ptr<asiodnp3::IMasterSession> proxy) :
	MasterOperationsAdapter(proxy.get()),
	m_master(new std::shared_ptr<asiodnp3::IMasterSession>(proxy))
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

bool MasterSessionAdapter::IsSameSession(IMasterSession^ other)
{
	if (other->GetType() != MasterSessionAdapter::typeid)
	{
		return false;
	}

	auto ref = (MasterSessionAdapter^)(other);

	// perform shared_ptr equality
	return ref->m_master == m_master;
}

void MasterSessionAdapter::BeginShutdown()
{
	(*m_master)->BeginShutdown();
}

}}}


