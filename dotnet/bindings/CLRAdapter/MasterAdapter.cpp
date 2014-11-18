
#include "Stdafx.h"
#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "MasterScanAdapter.h"
#include "Conversions.h"

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

			IMasterScan^ MasterAdapter::AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddAllObjectsScan(gvid, Conversions::ConvertTimespan(period));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddClassScan(ClassField field, System::TimeSpan period)
			{
				auto scan = pMaster->AddClassScan(Conversions::ConvertClassField(field), Conversions::ConvertTimespan(period));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddRangeScan(gvid, start, stop, Conversions::ConvertTimespan(period));
				return gcnew MasterScanAdapter(scan);
			}

			void MasterAdapter::ScanAllObjects(System::Byte group, System::Byte variation)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				pMaster->ScanAllObjects(gvid);
			}

			void MasterAdapter::ScanClasses(ClassField field)
			{
				pMaster->ScanClasses(Conversions::ConvertClassField(field));
			}

			void MasterAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				pMaster->ScanRange(gvid, start, stop);
			}

			void MasterAdapter::Write(TimeAndInterval^ value, System::UInt16 index)
			{				
				pMaster->Write(Conversions::ConvertMeas(value), index);
			}

		}
	}
}