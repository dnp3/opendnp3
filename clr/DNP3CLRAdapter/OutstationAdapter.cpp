#include "OutstationAdapter.h"
#include "OutstationLoaderAdapter.h"
#include "DeleteAnything.h"

namespace DNP3
{
namespace Adapter
{


OutstationAdapter::OutstationAdapter(asiodnp3::IOutstation* apOutstation) :
	mpOutstation(apOutstation),
	mLoaderAdapter(gcnew OutstationLoaderAdapter(apOutstation->GetLoader()))
{}

DNP3::Interface::IMeasurementLoader^ OutstationAdapter::GetLoader()
{
	return mLoaderAdapter;
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
