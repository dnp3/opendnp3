#ifndef __I_MASTER_H_
#define __I_MASTER_H_

#include "IStack.h"
#include "MasterScan.h"

#include <openpal/TimeDuration.h>

namespace opendnp3
{

class ICommandProcessor;

/** Interface that represents a running master.
* To get a command processor interface to execute controls on the master:-
\code
	ICommandProcessor* pCmdProcessor = pMaster->GetCommandProcessor();
\endcode
*/
class IMaster : public IStack
{
public:
	IMaster(openpal::Logger& arLogger, std::function<void (bool)> aEnableDisableFunc): 
		IStack(arLogger, aEnableDisableFunc)
	{}

	virtual ~IMaster() {}

	/**
	*  Returns a master scan object representing the configured integrity scan
	*/
	virtual MasterScan GetIntegrityScan() = 0;
	
	/**
	* Add a class-based scan to the master
	* @return A proxy class used to manipulate the scan
	*/
	virtual MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate) = 0;

	/**
	* Get a command processor interface to execute controls on the master
	* @return Interface used to invoke commands
	*/
	virtual ICommandProcessor* GetCommandProcessor() = 0;
};

}

#endif

