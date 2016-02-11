#ifndef CLR_DNP3_MASTER_SESSION_ADAPTER_H
#define	CLR_DNP3_MASTER_SESSION_ADAPTER_H

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

using namespace Automatak::DNP3::Interface;

#include <asiodnp3/IGPRSMaster.h>
#include <memory>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class MasterSessionAdapter sealed : IMasterSession
			{
			public:

				MasterSessionAdapter(std::shared_ptr<asiodnp3::IGPRSMaster> proxy);

				virtual IStackStatistics^ GetStackStatistics();				

				virtual void BeginShutdown();

				virtual Task<TaskCompletion>^ Scan(IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual Task<TaskCompletion>^ ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config);

				virtual Task<TaskCompletion>^ ScanClasses(ClassField field, TaskConfig^ config);

				virtual Task<TaskCompletion>^ ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config);

				virtual Task<TaskCompletion>^ Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config);

				virtual Task<RestartResultType^>^ Restart(RestartType restartType, TaskConfig^ config);

				virtual Task<TaskCompletion>^ PerformFunction(System::String^ name, FunctionCode func, IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config);

				/// --- implement ICommandProcessor ----

				virtual Task<CommandTaskResult^>^ SelectAndOperate(ICommandHeaders^ headers, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ DirectOperate(ICommandHeaders^ headers, TaskConfig^ config);

				virtual Task<CommandTaskResult^>^ SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config);

				virtual Task<CommandTaskResult^>^ DirectOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config);
				virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config);						

			private:
				
				std::shared_ptr<asiodnp3::IGPRSMaster>* proxy;
			};

		}
	}
}

#endif
