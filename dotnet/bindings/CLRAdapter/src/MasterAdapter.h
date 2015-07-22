#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include <asiodnp3/IMaster.h>

#include "CommandCallbackAdapter.h"
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
			
				virtual void Scan(IEnumerable<Header^>^ headers, TaskConfig^ config);

				virtual void ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config);

				virtual void ScanClasses(ClassField field, TaskConfig^ config);
				
				virtual void ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config);

				virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config);

				virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config);
				
				virtual void Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config);
				

				virtual IFuture<CommandResponse>^ SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputInt32^ command, System::UInt32 index);				
				virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputInt16^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputFloat32^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputDouble64^ command, System::UInt32 index);

				virtual IFuture<CommandResponse>^ DirectOperate(ControlRelayOutputBlock^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputInt32^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputInt16^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputFloat32^ command, System::UInt32 index);
				virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputDouble64^ command, System::UInt32 index);

			private:

				template <class T>
				IFuture<CommandResponse>^ SelectAndOperateT(T^ command, System::UInt32 index)
				{
					auto future = gcnew Future<CommandResponse>();
					auto cmd = Conversions::ConvertCommand(command);
					auto pCallback = new CommandCallbackAdapter(future, true);
					pMaster->SelectAndOperate(cmd, index, *pCallback);
					return future;
				}

				template <class T>
				IFuture<CommandResponse>^ DirectOperateT(T^ command, System::UInt32 index)
				{
					auto future = gcnew Future<CommandResponse>();
					auto cmd = Conversions::ConvertCommand(command);
					auto pCallback = new CommandCallbackAdapter(future, true);
					pMaster->DirectOperate(cmd, index, *pCallback);
					return future;
				}			
				
				asiodnp3::IMaster* pMaster;				
			};

		}
	}
}
#endif
