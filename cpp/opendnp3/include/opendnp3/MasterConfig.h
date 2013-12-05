#ifndef __MASTER_CONFIG_H_
#define __MASTER_CONFIG_H_

#include "DNPConstants.h"
#include "PointClass.h"

#include <openpal/TimeDuration.h>
#include <vector>

namespace opendnp3
{


/**
Configuration information for the dnp3 master
*/
struct MasterConfig {

	/// Default constructor
	MasterConfig() :
		FragSize(DEFAULT_FRAG_SIZE),
		UseNonStandardVtoFunction(false),
		AllowTimeSync(true),
		DoUnsolOnStartup(false),
		EnableUnsol(true),
		UnsolClassMask(PC_ALL_EVENTS),
		IntegrityRate(openpal::TimeDuration::Seconds(5)),
		TaskRetryRate(openpal::TimeDuration::Seconds(5))
	{}

	/// Maximum fragment size to use for requests
	size_t FragSize;

	/// Using FunctionCode::WRITE is a problem with vto because the spec won't allow it to retry
	bool UseNonStandardVtoFunction;

	/// If true, the master will do time syncs when it sees the time IIN bit from the slave
	bool AllowTimeSync;

	/// If true, the master will enable/disable unsol on startup
	bool DoUnsolOnStartup;

	/// If DoUnsolOnStartup == true, the master will use this bit to decide wether to enable (true) or disable (false)
	bool EnableUnsol;

	///	Bitwise mask used determine which classes are enabled/disabled for unsol
	int UnsolClassMask;

	/// Period for integrity scans (class 0), -1 for non periodic
	openpal::TimeDuration IntegrityRate;

	/// Time delay between task retries
	openpal::TimeDuration TaskRetryRate;
};

}

#endif

