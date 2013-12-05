#ifndef __DNP3_MANAGER_H_
#define __DNP3_MANAGER_H_

#include <string>
#include <set>
#include <stdint.h>
#include <memory>
#include <functional>

#include <openpal/Logger.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/TimeDuration.h>

#include "DestructorHook.h"



/*! \mainpage opendnp3

\section Introduction

Companion documentation that describes theory of operation is located on the <a href="http://www.github.com/automatakllc/dnp3/wiki">project wiki</a>.

A simple application can be written as follows:

\code
#include <opendnp3/DNP3Manger.h>

using namespace opendnp3;

int main(int argc, char* argv[])
{
	DNP3Manager manager(1); // instantiate the root object specifying how many threads to use in the pool

	// start adding communication channels.... Check out the wiki.
}
\endcode

*/

// pre-declare EVERYTHING possible to minimize includes for CLR/Java wrappers
namespace opendnp3
{

class IChannel;
class DNP3Channel;

/**
The root class for all dnp3 applications. Used to retrieve communication channels on
which masters / outstations can be bound. The stack is event-based and driven by a thread pool.
All callbacks come from a thread in the pool.

\code
	// instantiate the root object specifying how many threads to use in the pool
	DNP3Manager manager(std::thread::hardware_concurrency());
\endcode

*/
class DNP3Manager : public DestructorHook
{
public:

	DNP3Manager();

	~DNP3Manager();

	IChannel* CreateChannel(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, openpal::IPhysicalLayerAsync* apPhys);

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

private:

	void OnChannelShutdownCallback(DNP3Channel* apChannel);

	std::set<DNP3Channel*> mChannels;
};

}

#endif
