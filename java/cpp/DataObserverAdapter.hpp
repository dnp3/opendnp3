#ifndef __DATA_OBSERVER_ADAPTER_H_
#define __DATA_OBSERVER_ADAPTER_H_

#include <jni.h>
#include <opendnp3/DataInterfaces.h>

class DataObserverAdapter : public opendnp3::IDataObserver
{
	public:
	DataObserverAdapter(JavaVM* apJVM, jobject aProxy);

	protected:

	void _Start();
	void _Update(const opendnp3::Binary& arMeas, size_t aIndex);
	void _Update(const opendnp3::Analog& arMeas, size_t aIndex);
	void _Update(const opendnp3::Counter& arMeas, size_t aIndex);
	void _Update(const opendnp3::SetpointStatus& arMeas, size_t aIndex);
	void _Update(const opendnp3::ControlStatus& arMeas, size_t aIndex);
	void _End();

	private:

	JNIEnv* GetEnv();

	JavaVM* mpJVM;
	jobject mProxy;

	jmethodID mStartId;
	jmethodID mEndId;

	// BinaryInput
	jmethodID mUpdateBinaryInput;
	
	// AnalogInput
	jmethodID mUpdateAnalogInput;
	
	// Counter
	jmethodID mUpdateCounter;
	
	// BinaryOutputStatus
	jmethodID mUpdateBinaryOutputStatus;
	
	// AnalogOutputStatus
	jmethodID mUpdateAnalogOutputStatus;	
};

#endif
