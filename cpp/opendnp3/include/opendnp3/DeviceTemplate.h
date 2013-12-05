#ifndef __DEVICE_TEMPLATE_H_
#define __DEVICE_TEMPLATE_H_

#include <vector>
#include <sstream>

#include "IDataObserver.h"
#include "DeviceTemplateTypes.h"

namespace opendnp3
{

/** Configuration structure that defines the default layout of the database

	The indices of the points are implicit based on their
	position within the vectors below.
*/
struct DeviceTemplate {
	DeviceTemplate(size_t aNumBinary = 0,
	               size_t aNumAnalog = 0,
	               size_t aNumCounter = 0,
	               size_t aNumControlStatus = 0,
	               size_t aNumSetpointStatus = 0);


	/// list of binary point properties
	std::vector<EventPointRecord> mBinary;
	/// list of counter point properties
	std::vector<EventPointRecord> mCounter;
	/// list of analog point properties
	std::vector<DeadbandPointRecord> mAnalog;
	/// list of control status point properties
	std::vector<PointRecord> mControlStatus;
	/// list of control status point properties
	std::vector<PointRecord> mSetpointStatus;

	/// Controls how the quality is initialized. If false, the restart quality flag is set. If true online is set.
	bool mStartOnline;

	/// Write the initial state of a database to an observer
	void Publish(IDataObserver*);

private:

	template <class T>
	static void InitObserver(IDataObserver* apObs, size_t aNum) {
		for(size_t i = 0; i < aNum; ++i) {
			T val;
			apObs->Update(val, i);
		}
	}

};

}

#endif
