#include "com_automatak_dnp3_impl_MasterImpl.h"

#include "jni/JCache.h"

#include "asiodnp3/IMaster.h"

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_enable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Enable();
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_disable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Disable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Shutdown();
}
