
#include "Stdafx.h"

#include "Conversions.h"
#include "MasterAdapter.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{				

				MasterAdapter::MasterAdapter(asiodnp3::IMaster* apMaster) : pMaster(apMaster)
				{}				

				IStackStatistics^ MasterAdapter::GetStackStatistics()
				{
					auto stats = pMaster->GetStackStatistics();
					return Conversions::ConvertStackStats(stats);
				}

				void MasterAdapter::Enable()
				{
					pMaster->Enable();
				}

				void MasterAdapter::Disable()
				{
					pMaster->Disable();
				}

				void MasterAdapter::Shutdown()
				{
					pMaster->Shutdown();
				}
			
				Task<TaskCompletion>^ MasterAdapter::Scan(IEnumerable<Header^>^ headers, TaskConfig^ config)
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
					pMaster->Scan(vec, MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				Task<TaskCompletion>^ MasterAdapter::ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config)
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMaster->ScanAllObjects(opendnp3::GroupVariationID(group, variation), MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				Task<TaskCompletion>^ MasterAdapter::ScanClasses(ClassField field, TaskConfig^ config)
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMaster->ScanClasses(Conversions::ConvertClassField(field), MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}
				
				Task<TaskCompletion>^ MasterAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config)
				{
					opendnp3::GroupVariationID gvid(group, variation);
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMaster->ScanRange(gvid, start, stop, MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				Task<TaskCompletion>^ MasterAdapter::Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config)
				{
					auto proxy = gcnew TaskCompletionProxy(config->callback);
					pMaster->Write(Conversions::ConvertMeas(value), index, MasterConversions::Convert(config, proxy));
					return proxy->CompletionTask;
				}

				IMasterScan^ MasterAdapter::AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config)
				{
					auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
					auto scan = pMaster->AddScan(Conversions::ConvertTimespan(period), vec, MasterConversions::Convert(config));
					return gcnew MasterScanAdapter(scan);
				}

				IMasterScan^ MasterAdapter::AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config)
				{
					opendnp3::GroupVariationID gvid(group, variation);
					auto scan = pMaster->AddAllObjectsScan(gvid, Conversions::ConvertTimespan(period), MasterConversions::Convert(config));
					return gcnew MasterScanAdapter(scan);
				}

				IMasterScan^ MasterAdapter::AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config)
				{
					auto scan = pMaster->AddClassScan(Conversions::ConvertClassField(field), Conversions::ConvertTimespan(period), MasterConversions::Convert(config));
					return gcnew MasterScanAdapter(scan);
				}

				IMasterScan^ MasterAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config)
				{
					opendnp3::GroupVariationID gvid(group, variation);
					auto scan = pMaster->AddRangeScan(gvid, start, stop, Conversions::ConvertTimespan(period), MasterConversions::Convert(config));
					return gcnew MasterScanAdapter(scan);
				}
												
				Task<CommandResponse>^ MasterAdapter::SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->SelectAndOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputInt32^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->SelectAndOperateT(command, index, config);
				}
				
				Task<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputInt16^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->SelectAndOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputFloat32^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->SelectAndOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputDouble64^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->SelectAndOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::DirectOperate(ControlRelayOutputBlock^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->DirectOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputInt32^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->DirectOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputInt16^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->DirectOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputFloat32^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->DirectOperateT(command, index, config);
				}

				Task<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputDouble64^ command, System::UInt32 index, TaskConfig^ config)
				{
					return this->DirectOperateT(command, index, config);
				}			
		}
	}
}

