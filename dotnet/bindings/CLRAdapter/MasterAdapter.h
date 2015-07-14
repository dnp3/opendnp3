#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

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
			
				virtual void Scan(IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual void ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config);

				virtual void ScanClasses(ClassField field, TaskConfig^ config);
				
				virtual void ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config);

				virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config);
				
				virtual void Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config);

			private:

				static std::vector<asiodnp3::Header> ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers);

				static bool Convert(Header^ header, asiodnp3::Header& output);

				static asiodnp3::Header Convert(Header^ header);
				static asiodnp3::Header ConvertCount8(CountHeader^ header);
				static asiodnp3::Header ConvertCount16(CountHeader^ header);
				static asiodnp3::Header ConvertRange8(RangeHeader^ header);
				static asiodnp3::Header ConvertRange16(RangeHeader^ header);

				static opendnp3::TaskConfig Convert(TaskConfig^ config);
				static opendnp3::TaskId Convert(TaskId^ taskId);
				static opendnp3::ITaskCallback* CreateTaskCallback(ITaskCallback^ callback);
				

				asiodnp3::IMaster* pMaster;
				ICommandProcessor^ commandAdapter;
			};

		}
	}
}
#endif
