#ifndef __STARTUP_TASKS_H_
#define __STARTUP_TASKS_H_

#include <openpal/Visibility.h>
#include <openpal/IUTCTimeSource.h>

#include "MasterTaskBase.h"


namespace opendnp3
{

class ITimeSource;

// Clears the outstation IIN restart bit
class DLL_LOCAL ClearRestartIIN : public SimpleRspBase
{
public:
	ClearRestartIIN(openpal::Logger& arLogger);

	void ConfigureRequest(APDU& arAPDU);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "ClearRestartIIN";
	}
#endif
};

// Enables or disables unsolicited reporting
class DLL_LOCAL ConfigureUnsol : public SimpleRspBase
{
public:
	ConfigureUnsol(openpal::Logger&);

	void Set(bool aIsEnable, int aClassMask);

	void ConfigureRequest(APDU& arAPDU);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "ConfigureUnsol";
	}
#endif

private:
	bool mIsEnable;
	int mClassMask;
};

// Synchronizes the time on the outstation
class DLL_LOCAL TimeSync : public SingleRspBase
{
public:
	TimeSync(openpal::Logger&, openpal::IUTCTimeSource*);

	// override Init
	void Init();
	void ConfigureRequest(APDU& arAPDU);
	TaskResult _OnFinalResponse(const APDU&);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "TimeSync";
	}
#endif

private:
	openpal::IUTCTimeSource* mpTimeSrc;

	// < 0 implies the delay measure hasn't happened yet
	int64_t mDelay;

	// what time we sent the delay meas
	openpal::UTCTimestamp mStart;
};

} //ens ns

#endif

