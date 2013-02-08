#include "com_automatak_dnp3_impl_VTOEndpointImpl.h"

#include <opendnp3/IVtoEndpoint.h>

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_VTOEndpointImpl_shutdown_1native_1endpoint
  (JNIEnv *, jobject, jlong ptr)
{
	auto pEndpoint = (opendnp3::IVtoEndpoint*) ptr;
	pEndpoint->Shutdown();
}


