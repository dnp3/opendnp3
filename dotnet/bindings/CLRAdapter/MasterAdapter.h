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
			
				virtual void Scan(IEnumerable<Header^>^ headers, ITaskCallback^ callback, int userId);

				virtual void ScanAllObjects(System::Byte group, System::Byte variation, ITaskCallback^ callback, int userId);

				virtual void ScanClasses(ClassField field, ITaskCallback^ callback, int userId);
				
				virtual void ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, ITaskCallback^ callback, int userId);

				virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, ITaskCallback^ callback, int userId);

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, ITaskCallback^ callback, int userId);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, ITaskCallback^ callback, int userId);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, ITaskCallback^ callback, int userId);
				
				virtual void Write(TimeAndInterval^ value, System::UInt16 index, ITaskCallback^ callback, int userId);

			private:

				static std::vector<asiodnp3::Header> ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers);

				static bool Convert(Header^ header, asiodnp3::Header& output);

				static asiodnp3::Header Convert(Header^ header);
				static asiodnp3::Header ConvertCount8(CountHeader^ header);
				static asiodnp3::Header ConvertCount16(CountHeader^ header);
				static asiodnp3::Header ConvertRange8(RangeHeader^ header);
				static asiodnp3::Header ConvertRange16(RangeHeader^ header);

				static opendnp3::ITaskCallback* CreateTaskCallback(ITaskCallback^ callback);

				asiodnp3::IMaster* pMaster;
				ICommandProcessor^ commandAdapter;
			};

		}
	}
}
#endif
