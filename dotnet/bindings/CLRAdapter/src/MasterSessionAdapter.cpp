#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {


MasterSessionAdapter::MasterSessionAdapter(asiodnp3::IMasterSession* proxy) :
	MasterOperationsAdapter(proxy),
	m_master(proxy)
{

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
	m_master->BeginShutdown();
}

}}}


