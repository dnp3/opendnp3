#ifndef __STACK_STATE_CALLBACK_H_
#define __STACK_STATE_CALLBACK_H_

using namespace System::Collections::ObjectModel;
using namespace DNP3::Interface;


#include <opendnp3/gen/StackState.h>

#include <functional>
#include <vcclr.h>

namespace DNP3
{
namespace Adapter
{

void CallbackStackStateListener(opendnp3::StackState aState, gcroot < System::Action<StackState> ^ > * apCallback);

}
}

#endif
