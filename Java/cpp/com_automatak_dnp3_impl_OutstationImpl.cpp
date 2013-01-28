#include "com_automatak_dnp3_impl_OutstationImpl.h"

#include <DNP3/IOutstation.h>

#include <iostream>

using namespace apl;
using namespace apl::dnp;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_shutdown_1native
  (JNIEnv *, jobject, jlong ptr)
{
	auto pOutstation = (IOutstation*) ptr;
	pOutstation->Shutdown();
}

