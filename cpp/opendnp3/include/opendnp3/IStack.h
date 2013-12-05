#ifndef __I_STACK_H_
#define __I_STACK_H_

#include <functional>
#include <string>

#include <openpal/Logger.h>

#include "DestructorHook.h"
#include "gen/StackState.h"

namespace openpal
{
class IPhysicalLayerAsync;
}


namespace opendnp3
{

/**
* Base class for masters or outstations. Can be used to bind a vto endpoint or shutdown.
*/
class IStack : public DestructorHook
{
public:
	IStack(openpal::Logger& arLogger, std::function<void (bool)> aEnableDisableFunc);
	virtual ~IStack();

	/**
	* Add a listener for changes to the stack state. All callbacks come from the thread pool.
	* An immediate callback will be made with the current state.
	*
	* @param aListener Functor to call back with the state enumeration
	*/
	virtual void AddStateListener(std::function<void (StackState)> aListener) = 0;

	virtual openpal::IExecutor* GetExecutor() = 0;

	/**
	* Enable communications
	*/
	void Enable();

	/**
	* Enable communications
	*/
	void Disable();

	/** 
	* Synchronously shutdown the endpoint
    * No more calls are allowed after this call.
	*/
	virtual void Shutdown() = 0;

private:

	openpal::Logger mLogger;
	std::function<void (bool)> mEnableDisableFunc;
};

}

#endif
