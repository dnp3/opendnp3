

#include "MasterAdapter.h"
#include "Conversions.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			MasterAdapter::MasterAdapter(asiodnp3::IMaster* master) : MasterOperationsAdapter(master), master(master)
			{}

			void MasterAdapter::Enable()
			{
				master->Enable();
			}

			void MasterAdapter::Disable()
			{
				master->Disable();
			}

			void MasterAdapter::Shutdown()
			{
				master->Shutdown();
			}
		}
	}
}

