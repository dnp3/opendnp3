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

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_get_1native_1data_1observer
  (JNIEnv *, jobject, jlong ptr)
{
	auto pOutstation = (IOutstation*) ptr;
	return (jlong) pOutstation->GetDataObserver();
}