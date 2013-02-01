#include "com_automatak_dnp3_impl_DataObserverImpl.h"

#include <APL/DataInterfaces.h>

using namespace apl;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1start
  (JNIEnv* apEnv, jobject, jlong observer)
{
	auto pObs = (IDataObserver*) observer;
	pObs->Start();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1bi
  (JNIEnv *, jobject, jlong observer, jboolean val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Binary meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1ai
  (JNIEnv *, jobject, jlong observer, jdouble val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Analog meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1c
  (JNIEnv *, jobject, jlong observer, jlong val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Counter meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1bos
  (JNIEnv *, jobject, jlong observer, jboolean val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	ControlStatus meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1aos
  (JNIEnv *, jobject, jlong observer, jdouble val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	SetpointStatus meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1end
  (JNIEnv *, jobject, jlong observer)
{
	auto pObs = (IDataObserver*) observer;
	pObs->End();
}

