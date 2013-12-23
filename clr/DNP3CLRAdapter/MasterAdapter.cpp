#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "StackStateCallback.h"
#include "DeleteAnything.h"
#include "MasterScanAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{

MasterAdapter::MasterAdapter(opendnp3::IMaster* apMaster) :
	mpMaster(apMaster),
	mCommandAdapter(gcnew CommandProcessorAdapter(apMaster->GetCommandProcessor()))
{}

void MasterAdapter::AddStateListener(System::Action<StackState> ^ aListener)
{
	auto pListener = new gcroot < System::Action<StackState> ^ > (aListener);
	mpMaster->AddDestructorHook(std::bind(&DeleteAnything < gcroot < System::Action<StackState> ^ >> , pListener));
	mpMaster->AddStateListener(std::bind(&CallbackStackStateListener, std::placeholders::_1, pListener));
}

ICommandProcessor ^ MasterAdapter::GetCommandProcessor()
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
	mpMaster->Shutdown();
}

IMasterScan^ MasterAdapter::GetIntegrityScan()
{
	return gcnew MasterScanAdapter(mpMaster->GetIntegrityScan());
}

IMasterScan^ MasterAdapter::AddClassScan(int aClassMask, System::TimeSpan period, System::TimeSpan taskRetryPeriod)
{
	auto scan = mpMaster->AddClassScan(aClassMask, Conversions::convertTimeDuration(period),  Conversions::convertTimeDuration(taskRetryPeriod));
	return gcnew MasterScanAdapter(scan);
}

}
}


