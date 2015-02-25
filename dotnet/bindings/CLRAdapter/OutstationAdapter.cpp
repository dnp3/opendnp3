
#include "Stdafx.h"
#include "OutstationAdapter.h"

#include "ChangeSetAdapter.h"
#include "Conversions.h"

#include <opendnp3/outstation/Database.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			OutstationAdapter::OutstationAdapter(asiodnp3::IOutstation* pOutstation_) : pOutstation(pOutstation_)				
			{}

			void OutstationAdapter::LoadChanges(System::Action<IDatabase^>^ changeFun)
			{
				auto adapter = gcnew ChangeSetAdapter(*pOutstation);
				changeFun->Invoke(adapter);
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
