#include "OutstationAdapter.h"
#include "SlaveDataObserverAdapter.h"
#include "DeleteAnything.h"

namespace DNP3
{
namespace Adapter
{


OutstationAdapter::OutstationAdapter(opendnp3::IOutstation* apOutstation) :
	mpOutstation(apOutstation),
	mDataObserverAdapter(gcnew SlaveDataObserverAdapter(apOutstation->GetDataObserver()))
{}

IDataObserver^ OutstationAdapter::GetDataObserver()
{
	return mDataObserverAdapter;
}

void OutstationAdapter::SetNeedTimeIIN()
{
	mpOutstation->SetNeedTimeIIN();
}

void OutstationAdapter::Shutdown()
{
	mpOutstation->BeginShutdown();
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
