#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "DeleteAnything.h"
#include "MasterScanAdapter.h"
#include "Conversions.h"

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
	pMaster->BeginShutdown();
}

IMasterScan^ MasterAdapter::AddClassScan(int aClassMask, System::TimeSpan period)
{
	auto scan = pMaster->AddClassScan(aClassMask, Conversions::convertTimespan(period));
	return gcnew MasterScanAdapter(scan);
}

IMasterScan^ MasterAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period)
{
	opendnp3::GroupVariationID gvid(group, variation);
	auto scan = pMaster->AddRangeScan(gvid, start, stop, Conversions::convertTimespan(period));
	return gcnew MasterScanAdapter(scan);
}

}
}


