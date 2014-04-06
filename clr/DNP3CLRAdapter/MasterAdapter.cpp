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
	mpMaster(apMaster),
	mCommandAdapter(gcnew CommandProcessorAdapter(apMaster->GetCommandProcessor()))
{}

ICommandProcessor^ MasterAdapter::GetCommandProcessor()
{
	return mCommandAdapter;
}

void MasterAdapter::Enable()
{
	mpMaster->Enable();
}

void MasterAdapter::Disable()
{
	mpMaster->Disable();
}

void MasterAdapter::Shutdown()
{
	mpMaster->BeginShutdown();
}

IMasterScan^ MasterAdapter::GetIntegrityScan()
{
	return gcnew MasterScanAdapter(mpMaster->GetIntegrityScan());
}

IMasterScan^ MasterAdapter::AddClassScan(int aClassMask, System::TimeSpan period, System::TimeSpan taskRetryPeriod)
{
	auto scan = mpMaster->AddClassScan(aClassMask, Conversions::convertTimespan(period),  Conversions::convertTimespan(taskRetryPeriod));
	return gcnew MasterScanAdapter(scan);
}

}
}


