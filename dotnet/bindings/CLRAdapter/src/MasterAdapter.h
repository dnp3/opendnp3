#ifndef OPENDNP3CLR_MASTER_ADAPTER_H
#define OPENDNP3CLR_MASTER_ADAPTER_H

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include <asiodnp3/IMaster.h>

#include "CallbackAdapters.h"
#include "MasterConversions.h"
#include "MasterScanAdapter.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{			
			private ref class MasterAdapter abstract
			{
			public:

				MasterAdapter(asiodnp3::IMaster* apMaster);

				virtual IStackStatistics^ GetStackStatistics();

				virtual void Enable();

				virtual void Disable();

				virtual void Shutdown();
			
				virtual Task<TaskCompletion>^ Scan(IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual Task<TaskCompletion>^ ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config);

				virtual Task<TaskCompletion>^ ScanClasses(ClassField field, TaskConfig^ config);
				
				virtual Task<TaskCompletion>^ ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config);

				virtual Task<TaskCompletion>^ Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config);

				virtual Task<TaskCompletion>^ PerformFunction(System::String^ name, FunctionCode func, IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config);												

				virtual Task<CommandResponse>^ SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ SelectAndOperate(AnalogOutputInt32^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ SelectAndOperate(AnalogOutputInt16^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ SelectAndOperate(AnalogOutputFloat32^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ SelectAndOperate(AnalogOutputDouble64^ command, System::UInt32 index, TaskConfig^ config);

				virtual Task<CommandResponse>^ DirectOperate(ControlRelayOutputBlock^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ DirectOperate(AnalogOutputInt32^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ DirectOperate(AnalogOutputInt16^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ DirectOperate(AnalogOutputFloat32^ command, System::UInt32 index, TaskConfig^ config);
				virtual Task<CommandResponse>^ DirectOperate(AnalogOutputDouble64^ command, System::UInt32 index, TaskConfig^ config);

			private:

				template <class T>
				Task<CommandResponse>^ SelectAndOperateT(T^ command, System::UInt32 index, TaskConfig^ config)
				{														
					auto tcs = gcnew TaskCompletionSource<CommandResponse>();
					auto cmd = Conversions::ConvertCommand(command);
					pMaster->SelectAndOperate(cmd, index, CallbackAdapters::Get(tcs), MasterConversions::Convert(config));
					return tcs->Task;
				}

				template <class T>
				Task<CommandResponse>^ DirectOperateT(T^ command, System::UInt32 index, TaskConfig^ config)
				{				
					auto tcs = gcnew TaskCompletionSource<CommandResponse>();
					auto cmd = Conversions::ConvertCommand(command);
					pMaster->DirectOperate(cmd, index, CallbackAdapters::Get(tcs), MasterConversions::Convert(config));
					return tcs->Task;
				}			
				
				asiodnp3::IMaster* pMaster;				
			};

		}
	}
}
#endif
