
#include "Stdafx.h"
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
			
				void MasterAdapter::Scan(IEnumerable<Header^>^ headers, TaskConfig^ config)
				{
					auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
					pMaster->Scan(vec, MasterConversions::Convert(config));
				}

				void MasterAdapter::ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config)
				{
					pMaster->ScanAllObjects(opendnp3::GroupVariationID(group, variation), MasterConversions::Convert(config));
				}

				void MasterAdapter::ScanClasses(ClassField field, TaskConfig^ config)
				{
					pMaster->ScanClasses(Conversions::ConvertClassField(field), MasterConversions::Convert(config));
				}
				
				void MasterAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config)
				{
					opendnp3::GroupVariationID gvid(group, variation);
					pMaster->ScanRange(gvid, start, stop, MasterConversions::Convert(config));
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
				
				void MasterAdapter::Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config)
				{
					pMaster->Write(Conversions::ConvertMeas(value), index, MasterConversions::Convert(config));
				}
				

				IFuture<CommandResponse>^ MasterAdapter::SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt32 index)
				{
					return this->SelectAndOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputInt32^ command, System::UInt32 index)
				{
					return this->SelectAndOperateT(command, index);
				}
				
				IFuture<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputInt16^ command, System::UInt32 index)
				{
					return this->SelectAndOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputFloat32^ command, System::UInt32 index)
				{
					return this->SelectAndOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::SelectAndOperate(AnalogOutputDouble64^ command, System::UInt32 index)
				{
					return this->SelectAndOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::DirectOperate(ControlRelayOutputBlock^ command, System::UInt32 index)
				{
					return this->DirectOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputInt32^ command, System::UInt32 index)
				{
					return this->DirectOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputInt16^ command, System::UInt32 index)
				{
					return this->DirectOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputFloat32^ command, System::UInt32 index)
				{
					return this->DirectOperateT(command, index);
				}

				IFuture<CommandResponse>^ MasterAdapter::DirectOperate(AnalogOutputDouble64^ command, System::UInt32 index)
				{
					return this->DirectOperateT(command, index);
				}			
		}
	}
}

