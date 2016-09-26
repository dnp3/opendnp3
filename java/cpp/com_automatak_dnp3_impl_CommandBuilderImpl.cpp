#include "com_automatak_dnp3_impl_CommandBuilderImpl.h"

#include "opendnp3/master/CommandSet.h"

using namespace opendnp3;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_create_1native
(JNIEnv* env, jobject)
{
	return (jlong) new CommandSet();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_destroy_1native
  (JNIEnv* env, jobject, jlong native)
{
	delete (CommandSet*)native;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1crob_1native
(JNIEnv* env, jobject, jlong, jobject)
{

}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoI16_1native
(JNIEnv* env, jobject, jlong, jobject)
{

}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoI32_1native
(JNIEnv* env, jobject, jlong, jobject)
{

}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoF32_1native
(JNIEnv* env, jobject, jlong, jobject)
{

}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoD64_1native
(JNIEnv* env, jobject, jlong, jobject)
{

}

