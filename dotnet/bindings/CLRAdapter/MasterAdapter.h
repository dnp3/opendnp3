#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IMaster.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class MasterAdapter : IMaster
			{
			public:

				MasterAdapter(asiodnp3::IMaster* apMaster);

				virtual ICommandProcessor^ GetCommandProcessor();

				virtual IStackStatistics^ GetStackStatistics();

				virtual void Enable();

				virtual void Disable();

				virtual void Shutdown();

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period);

				virtual void ScanAllObjects(System::Byte group, System::Byte variation);

				virtual void ScanClasses(ClassField field);
				
				virtual void ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop);

				virtual void Write(TimeAndInterval^ value, System::UInt16 index);

			private:
				asiodnp3::IMaster* pMaster;
				ICommandProcessor^ commandAdapter;
			};

		}
	}
}
#endif
