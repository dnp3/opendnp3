
#include "com_automatak_dnp3_impl_OutstationImpl.h"

#include "asiodnp3/IOutstation.h"

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_enable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto outstation = (asiodnp3::IOutstation*) native;
	outstation->Enable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_disable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto outstation = (asiodnp3::IOutstation*) native;
	outstation->Disable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native)
{
	auto outstation = (asiodnp3::IOutstation*) native;
	outstation->Shutdown();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_apply_1native
(JNIEnv* env, jobject, jlong native, jlong nativeChangeSet)
{
	auto outstation = (asiodnp3::IOutstation*) native;
	auto changeSet = (asiodnp3::ChangeSet*) nativeChangeSet;
	outstation->Apply(*changeSet);
}
