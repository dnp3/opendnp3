/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include "com_automatak_dnp3_impl_DataObserverImpl.h"

#include <opendnp3/IDataObserver.h>

using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1start
(JNIEnv* apEnv, jobject, jlong observer)
{
	auto pObs = (IDataObserver*) observer;
	Transaction::Start(pObs);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1bi
(JNIEnv*, jobject, jlong observer, jboolean val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Binary meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1ai
(JNIEnv*, jobject, jlong observer, jdouble val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Analog meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1c
(JNIEnv*, jobject, jlong observer, jlong val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	Counter meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1fc
  (JNIEnv *, jobject, jlong observer, jlong val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	FrozenCounter meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1bos
(JNIEnv*, jobject, jlong observer, jboolean val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	ControlStatus meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1update_1aos
(JNIEnv*, jobject, jlong observer, jdouble val, jbyte qual, jlong time, jlong index)
{
	auto pObs = (IDataObserver*) observer;
	SetpointStatus meas(val, qual);
	meas.SetTime(time);
	pObs->Update(meas, index);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DataObserverImpl_native_1end
(JNIEnv*, jobject, jlong observer)
{
	auto pObs = (IDataObserver*) observer;
	Transaction::End(pObs);
}

