
#include "com_automatak_dnp3_impl_ChangeSetImpl.h"

#include "asiodnp3/ChangeSet.h"

using namespace opendnp3;
using namespace asiodnp3;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_create_1instance_1native
(JNIEnv* env, jobject)
{
	return (jlong) new ChangeSet();
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_destroy_1instance_1native
(JNIEnv* env, jobject, jlong native)
{
	delete (ChangeSet*) native;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1binary_1native
(JNIEnv* env, jobject, jlong native, jboolean value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(Binary(!!value, flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1double_1binary_1native
(JNIEnv* env, jobject, jlong native, jint value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(DoubleBitBinary(static_cast<DoubleBit>(value), flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1analog_1native
(JNIEnv* env, jobject, jlong native, jdouble value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(Analog(value, flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1counter_1native
(JNIEnv* env, jobject, jlong native, jlong value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(Counter(static_cast<uint32_t>(value), flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1frozen_1counter_1native
(JNIEnv* env, jobject, jlong native, jlong value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(FrozenCounter(static_cast<uint32_t>(value), flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1bo_1status_1native
(JNIEnv* env, jobject, jlong native, jboolean value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(BinaryOutputStatus(!!value, flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChangeSetImpl_update_1ao_1status_1native
 (JNIEnv* env, jobject, jlong native, jdouble value, jbyte flags, jlong time, jint index, jint mode)
{
	const auto changes = (ChangeSet*) native;
	changes->Update(AnalogOutputStatus(value, flags, DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

