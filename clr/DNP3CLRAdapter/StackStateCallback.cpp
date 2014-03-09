#include "StackStateCallback.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{
void CleanupCallback(gcroot < System::Action<StackState> ^ >* apCallback)
{
	delete apCallback;
}

void CallbackStackStateListener(opendnp3::StackState aState, gcroot < System::Action<StackState> ^ >* apCallback)
{
	(*apCallback)->Invoke(Conversions::convertStackState(aState));
}



}
}


