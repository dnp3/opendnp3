
#include "Stdafx.h"
#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "MasterScanAdapter.h"
#include "Conversions.h"
#include "TaskCallbackAdapter.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			MasterAdapter::MasterAdapter(asiodnp3::IMaster* apMaster) :
				pMaster(apMaster),
				commandAdapter(gcnew CommandProcessorAdapter(apMaster->GetCommandProcessor()))
			{}

			ICommandProcessor^ MasterAdapter::GetCommandProcessor()
			{
				return commandAdapter;
			}

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

			IMasterScan^ MasterAdapter::AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, ITaskCallback^ callback)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddAllObjectsScan(gvid, Conversions::ConvertTimespan(period), CreateTaskCallback(callback));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddClassScan(ClassField field, System::TimeSpan period, ITaskCallback^ callback)
			{
				auto scan = pMaster->AddClassScan(Conversions::ConvertClassField(field), Conversions::ConvertTimespan(period), CreateTaskCallback(callback));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, ITaskCallback^ callback)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddRangeScan(gvid, start, stop, Conversions::ConvertTimespan(period), CreateTaskCallback(callback));
				return gcnew MasterScanAdapter(scan);
			}

			void MasterAdapter::ScanAllObjects(System::Byte group, System::Byte variation, ITaskCallback^ callback)
			{				
				pMaster->ScanAllObjects(opendnp3::GroupVariationID(group, variation), CreateTaskCallback(callback));
			}

			void MasterAdapter::ScanClasses(ClassField field, ITaskCallback^ callback)
			{
				pMaster->ScanClasses(Conversions::ConvertClassField(field), CreateTaskCallback(callback));
			}

			void MasterAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, ITaskCallback^ callback)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				pMaster->ScanRange(gvid, start, stop, CreateTaskCallback(callback));
			}

			void MasterAdapter::Write(TimeAndInterval^ value, System::UInt16 index, ITaskCallback^ callback)
			{				
				pMaster->Write(Conversions::ConvertMeas(value), index, CreateTaskCallback(callback));
			}

			opendnp3::ITaskCallback* MasterAdapter::CreateTaskCallback(ITaskCallback^ callback)
			{
				return (callback == nullptr) ? nullptr : TaskCallbackAdapter::Create(callback);
			}
		}
	}
}