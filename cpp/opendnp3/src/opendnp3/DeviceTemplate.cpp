#include <opendnp3/DeviceTemplate.h>

namespace opendnp3
{

DeviceTemplate::DeviceTemplate(size_t aNumBinary,
                               size_t aNumAnalog,
                               size_t aNumCounter,
                               size_t aNumControlStatus,
                               size_t aNumSetpointStatus) :
	mStartOnline(false)
{
	this->mBinary.resize(aNumBinary);
	this->mAnalog.resize(aNumAnalog);
	this->mCounter.resize(aNumCounter);
	this->mControlStatus.resize(aNumControlStatus);
	this->mSetpointStatus.resize(aNumSetpointStatus);
}

void DeviceTemplate::Publish(IDataObserver* apObs)
{
	Transaction tr(apObs);
	InitObserver<Binary>(apObs, mBinary.size());
	InitObserver<Analog>(apObs, mAnalog.size());
	InitObserver<Counter>(apObs, mCounter.size());
	InitObserver<ControlStatus>(apObs, mControlStatus.size());
	InitObserver<SetpointStatus>(apObs, mSetpointStatus.size());
}

}


