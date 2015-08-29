

#include "OutstationAdapter.h"

#include "ChangeSetAdapter.h"
#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			OutstationAdapter::OutstationAdapter(asiodnp3::IOutstation* pOutstation_) : pOutstation(pOutstation_)				
			{}

			void OutstationAdapter::Load(IChangeSet^ changes)
			{
				auto adapter = gcnew ChangeSetAdapter(*pOutstation);
				changes->Apply(adapter);
				adapter->Apply();
			}			

			void OutstationAdapter::SetRestartIIN()
			{
				pOutstation->SetRestartIIN();
			}

			void OutstationAdapter::Shutdown()
			{
				pOutstation->Shutdown();
			}

			void OutstationAdapter::Enable()
			{
				pOutstation->Enable();
			}

			void OutstationAdapter::Disable()
			{
				pOutstation->Disable();
			}

			IStackStatistics^ OutstationAdapter::GetStackStatistics()
			{
				auto stats = pOutstation->GetStackStatistics();
				return Conversions::ConvertStackStats(stats);
			}

		}
	}
}
