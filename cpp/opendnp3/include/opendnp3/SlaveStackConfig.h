#ifndef __SLAVE_STACK_CONFIG_H_
#define __SLAVE_STACK_CONFIG_H_

#include "SlaveConfig.h"
#include "DeviceTemplate.h"
#include "AppConfig.h"
#include "LinkConfig.h"

namespace opendnp3
{

/** A composite configuration struct that contains all the config
	information for a dnp3 slave stack
*/
struct SlaveStackConfig {
	SlaveStackConfig() :
		link(false, false)
	{}

	/// Slave config
	SlaveConfig slave;
	/// Device template that specifies database layout
	DeviceTemplate device;
	/// Application layer config
	AppConfig app;
	/// Link layer config
	LinkConfig link;

};

}

#endif

