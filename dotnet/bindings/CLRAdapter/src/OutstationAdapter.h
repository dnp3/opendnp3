#ifndef __OUTSTATION_ADAPTER_H_
#define __OUTSTATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IOutstation.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class OutstationAdapter : IOutstation
			{
			public:

				OutstationAdapter(asiodnp3::IOutstation* pOutstation);

				virtual void LoadChanges(System::Action<IDatabase^>^ changeFun);

				virtual void SetRestartIIN();

				virtual void Shutdown();

				virtual void Enable();

				virtual void Disable();

				virtual IStackStatistics^ GetStackStatistics();

			private:
				asiodnp3::IOutstation* pOutstation;				
			};

		}
	}
}

#endif
