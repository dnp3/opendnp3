#include "DataObserverAdapter.h"


using namespace apl;

DataObserverAdapter::DataObserverAdapter(JavaVM* apJVM, jobject aProxy) :
mpJVM(apJVM),
mProxy(aProxy)
{

}

void DataObserverAdapter::_Start()
{
	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);	

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);				
	jmethodID mid = pEnv->GetMethodID(clazz, "start", "()V");
	assert(mid != NULL);	

	pEnv->CallVoidMethod(mProxy, mid);
}

void DataObserverAdapter::_Update(const Binary& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const Analog& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const Counter& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const SetpointStatus& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const ControlStatus& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_End()
{
	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);	

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);				
	jmethodID mid = pEnv->GetMethodID(clazz, "end", "()V");
	assert(mid != NULL);	

	pEnv->CallVoidMethod(mProxy, mid);
}
