#ifndef __MASTER_STACK_CONFIG_H_
#define __MASTER_STACK_CONFIG_H_

#include "MasterConfig.h"
#include "AppConfig.h"
#include "LinkConfig.h"

namespace opendnp3
{

/** A composite configuration struct that contains all the config
	information for a dnp3 master stack
*/
struct MasterStackConfig {
	MasterStackConfig() :
		link(true, false)
	{}

	/// Master config
	MasterConfig master;

	/// Application layer config
	AppConfig app;

	/// Link layer config
	LinkConfig link;


};

}

#endif

