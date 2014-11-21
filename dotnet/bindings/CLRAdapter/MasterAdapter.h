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

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, ITaskCallback^ callback);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, ITaskCallback^ callback);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, ITaskCallback^ callback);

				virtual void ScanAllObjects(System::Byte group, System::Byte variation, ITaskCallback^ callback);

				virtual void ScanClasses(ClassField field, ITaskCallback^ callback);
				
				virtual void ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, ITaskCallback^ callback);

				virtual void Write(TimeAndInterval^ value, System::UInt16 index, ITaskCallback^ callback);				

			private:

				static opendnp3::ITaskCallback* CreateTaskCallback(ITaskCallback^ callback);

				asiodnp3::IMaster* pMaster;
				ICommandProcessor^ commandAdapter;
			};

		}
	}
}
#endif
