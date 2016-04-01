

#include "MasterAdapter.h"
#include "Conversions.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			MasterAdapter::MasterAdapter(asiodnp3::IMaster* master) : MasterOperationsAdapter(master), m_master(master)
			{}

			void MasterAdapter::Enable()
			{
				m_master->Enable();
			}

			void MasterAdapter::Disable()
			{
				m_master->Disable();
			}

			void MasterAdapter::Shutdown()
			{
				m_master->Shutdown();
			}
		}
	}
}

