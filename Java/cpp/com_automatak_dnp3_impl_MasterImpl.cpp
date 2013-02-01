#include "com_automatak_dnp3_impl_MasterImpl.h"

#include <DNP3/IMaster.h>

using namespace apl;
using namespace apl::dnp;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_shutdown_1native
  (JNIEnv *, jobject, jlong ptr)
{
	auto pMaster = (IMaster*) ptr;
	pMaster->Shutdown();
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_MasterImpl_get_1native_1command_1processor
  (JNIEnv *, jobject, jlong ptr)
{
	auto pMaster = (IMaster*) ptr;
	return (jlong) pMaster->GetCommandProcessor();
}