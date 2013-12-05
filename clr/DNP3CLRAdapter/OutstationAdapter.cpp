#include "OutstationAdapter.h"
#include "SlaveDataObserverAdapter.h"
#include "StackStateCallback.h"
#include "DeleteAnything.h"

namespace DNP3
{
namespace Adapter
{


OutstationAdapter::OutstationAdapter(opendnp3::IOutstation* apOutstation) :
	mpOutstation(apOutstation),
	mDataObserverAdapter(gcnew SlaveDataObserverAdapter(apOutstation->GetDataObserver()))
{}

void OutstationAdapter::AddStateListener(System::Action<StackState> ^ aListener)
{
	auto pListener = new gcroot < System::Action<StackState> ^ > (aListener);
	mpOutstation->AddDestructorHook(std::bind(&DeleteAnything < gcroot < System::Action<StackState> ^ >> , pListener));
	mpOutstation->AddStateListener(std::bind(&CallbackStackStateListener, std::placeholders::_1, pListener));
}

IDataObserver ^ OutstationAdapter::GetDataObserver()
{
	return mDataObserverAdapter;
}

void OutstationAdapter::SetNeedTimeIIN()
{
	mpOutstation->SetNeedTimeIIN();
}

void OutstationAdapter::Shutdown()
{
	mpOutstation->Shutdown();
}


void OutstationAdapter::Enable()
{
	mpOutstation->Enable();
}

void OutstationAdapter::Disable()
{
	mpOutstation->Disable();
}

}
}
