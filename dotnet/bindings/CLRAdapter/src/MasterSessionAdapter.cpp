#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {


MasterSessionAdapter::MasterSessionAdapter(asiodnp3::IMasterSession* proxy) :
	MasterOperationsAdapter(proxy),
	m_master(proxy)
{

}

void MasterSessionAdapter::BeginShutdown()
{
	m_master->BeginShutdown();
}

}}}


