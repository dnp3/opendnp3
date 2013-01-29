#include "DataObserverAdapter.h"


using namespace apl;

#include <iostream>

DataObserverAdapter::DataObserverAdapter(JavaVM* apJVM, jobject aProxy) :
mpJVM(apJVM),
mProxy(aProxy)
{
	JNIEnv* pEnv = this->GetEnv();
	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);	

	mStartId = pEnv->GetMethodID(clazz, "start", "()V");
	assert(mStartId != NULL);
	mEndId = pEnv->GetMethodID(clazz, "end", "()V");
	assert(mEndId != NULL);


	mBinaryInputClass = pEnv->FindClass("com/automatak/dnp3/BinaryInput");
	assert(mBinaryInputClass != NULL);				   
	mInitBinaryInput = pEnv->GetMethodID(mBinaryInputClass, "<init>","(ZBJ)V");
	assert(mInitBinaryInput != NULL);
	mUpdateBinaryInput = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/BinaryInput;J)V");
	assert(mUpdateBinaryInput != NULL);

	mAnalogInputClass = pEnv->FindClass("com/automatak/dnp3/AnalogInput");
	assert(mAnalogInputClass != NULL);				   
	mInitAnalogInput = pEnv->GetMethodID(mAnalogInputClass, "<init>","(DBJ)V");
	assert(mInitAnalogInput != NULL);
	mUpdateAnalogInput = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/AnalogInput;J)V");
	assert(mUpdateAnalogInput != NULL);

	mCounterClass = pEnv->FindClass("com/automatak/dnp3/Counter");
	assert(mCounterClass != NULL);				   
	mInitCounter = pEnv->GetMethodID(mCounterClass, "<init>","(JBJ)V");
	assert(mInitCounter != NULL);
	mUpdateCounter = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/Counter;J)V");
	assert(mUpdateCounter != NULL);

	mBinaryOutputStatusClass = pEnv->FindClass("com/automatak/dnp3/BinaryOutputStatus");
	assert(mBinaryOutputStatusClass != NULL);				   
	mInitBinaryOutputStatus = pEnv->GetMethodID(mBinaryOutputStatusClass, "<init>","(ZBJ)V");
	assert(mInitBinaryOutputStatus != NULL);
	mUpdateBinaryOutputStatus = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/BinaryOutputStatus;J)V");
	assert(mUpdateBinaryOutputStatus != NULL);

	mAnalogOutputStatusClass = pEnv->FindClass("com/automatak/dnp3/AnalogOutputStatus");
	assert(mAnalogOutputStatusClass != NULL);				   
	mInitAnalogOutputStatus = pEnv->GetMethodID(mAnalogOutputStatusClass, "<init>","(DBJ)V");
	assert(mInitAnalogOutputStatus != NULL);
	mUpdateAnalogOutputStatus = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/AnalogOutputStatus;J)V");
	assert(mUpdateAnalogOutputStatus != NULL);
}

JNIEnv* DataObserverAdapter::GetEnv()
{
	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);
	return pEnv;
}

void DataObserverAdapter::_Start()
{
	GetEnv()->CallVoidMethod(mProxy, mStartId);	
}

void DataObserverAdapter::_Update(const Binary& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mBinaryInputClass, mInitBinaryInput, value, quality, timestamp);
	pEnv->CallVoidMethod(mProxy, mUpdateBinaryInput, meas, aIndex);
}

void DataObserverAdapter::_Update(const Analog& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mAnalogInputClass, mInitAnalogInput, value, quality, timestamp);
	pEnv->CallVoidMethod(mProxy, mUpdateAnalogInput, meas, aIndex);
}

void DataObserverAdapter::_Update(const Counter& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jlong value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mCounterClass, mInitCounter, value, quality, timestamp);
	pEnv->CallVoidMethod(mProxy, mUpdateCounter, meas, aIndex);
}

void DataObserverAdapter::_Update(const SetpointStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mAnalogOutputStatusClass, mInitAnalogOutputStatus, value, quality, timestamp);
	pEnv->CallVoidMethod(mProxy, mUpdateAnalogOutputStatus, meas, aIndex);	
}

void DataObserverAdapter::_Update(const ControlStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mBinaryOutputStatusClass, mInitBinaryOutputStatus, value, quality, timestamp);
	pEnv->CallVoidMethod(mProxy, mUpdateBinaryOutputStatus, meas, aIndex);	
}

void DataObserverAdapter::_End()
{
	GetEnv()->CallVoidMethod(mProxy, mEndId);
}
