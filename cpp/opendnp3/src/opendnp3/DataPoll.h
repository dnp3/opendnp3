#ifndef __DATA_POLL_H_
#define __DATA_POLL_H_

#include <opendnp3/MeasurementUpdate.h>
#include "MasterTaskBase.h"

#include <functional>
#include <openpal/Visibility.h>

namespace opendnp3
{

class IMeasurementHandler;

/**
 * Base class for all data acquistion polls
 */
class DLL_LOCAL DataPoll : public MasterTaskBase
{
public:

	DataPoll(openpal::Logger&, const std::function<void (MeasurementUpdate&)>& aUpdate);

private:

	void ReadData(const APDU&);

	//Implement MasterTaskBase
	TaskResult _OnPartialResponse(const APDU&);
	TaskResult _OnFinalResponse(const APDU&);

	std::function<void (MeasurementUpdate&)> mUpdateCallback;

};

/** Task that acquires class data from the outstation
*/
class DLL_LOCAL ClassPoll : public DataPoll
{
public:

	ClassPoll(openpal::Logger&, const std::function<void (MeasurementUpdate&)>& aUpdate);

	void Set(int aClassMask);

	//Implement MasterTaskBase
	void ConfigureRequest(APDU& arAPDU);
	virtual std::string Name() const {
		return "Class Poll";
	}

private:

	int mClassMask;

};

} //ens ns

/* vim: set ts=4 sw=4: */

#endif
