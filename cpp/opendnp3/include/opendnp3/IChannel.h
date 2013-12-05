#ifndef __I_CHANNEL_H_
#define __I_CHANNEL_H_

#include "gen/ChannelState.h"

#include "MasterStackConfig.h"
#include "SlaveStackConfig.h"
#include "DestructorHook.h"

#include <openpal/gen/LogLevel.h>
#include <openpal/IUTCTimeSource.h>

#include <functional>

namespace opendnp3
{

class IMeasurementHandler;
class IMaster;
class IOutstation;
class ICommandHandler;
class ITimeWriteHandler;

/**
* Represents a communication channel upon which masters and outstations can be bound.
* To add a master to a communication channel, where pClient is a channel pointer IChannel*\n
* Returns a pointer to the running master
\code
	IMaster* pMaster = pClient->AddMaster(
		"master",						// stack name
		LOG_LEVEL,						// log filter level
		PrintingDataObserver::Inst(),	// callback for data processing
		stackConfig						// stack configuration
	);
\endcode
* To add an outstation to a communication channel, where pServer is a channel pointer IChannel*\n
* Returns a pointer to the running outstation
\code
	IOutstation* pOutstation = pServer->AddOutstation(
		"outstation",								// stack name
		LOG_LEVEL,									// log filter level
		SuccessCommandHandler::Inst(),				// callback for command requests
		stackConfig									// stack configuration
	);
\endcode
*/
class IChannel : public DestructorHook
{
public:

	virtual ~IChannel() {}

	/**
	* Synchronously shutdown the channel.
	*/
	virtual void Shutdown() = 0;

	/**
	* Add a listener for changes to the channel state. All callbacks come from the thread pool.
	* An immediate callback will be made with the current state.
	*
	* @param aListener Functor to callback with the state enumeration
	*/
	virtual void AddStateListener(std::function<void (ChannelState)> aListener) = 0;


	/**
        * @return The execution context associated with the channel
        */
	virtual openpal::IExecutor* GetExecutor() = 0;

#ifndef OPENDNP3_NO_MASTER

	/**
	* Add a master to the channel
	*
	* @param arLoggerId Name that will be used in all log messages
	* @param aLevel Lowest log level that will be recorded
	* @param apPublisher Callback object for all received measurements
	* @param arCfg Configuration object that controls how the master behaves
	* @return interface representing the running master
	*/
	virtual IMaster* AddMaster(	const std::string& arLoggerId,
	                                openpal::LogLevel aLevel,
	                                IMeasurementHandler* apPublisher,
	                                openpal::IUTCTimeSource* apTimeSource,
	                                const MasterStackConfig& arCfg) = 0;

#endif

	/**
	* Add an outstation to the channel
	*
	* @param arLoggerId Name that will be used in all log messages
	* @param aLevel Lowest log level that will be recorded
	* @param apCmdHandler Callback object for handling command requests
	* @param arCfg Configuration object that controls how the outstation behaves
	* @return interface representing the running outstations
	*/
	virtual IOutstation* AddOutstation(	const std::string& arLoggerId,
	                                        openpal::LogLevel aLevel,
	                                        ICommandHandler* apCmdHandler,
	                                        ITimeWriteHandler* apTimeWriteHandler,
	                                        const SlaveStackConfig& arCfg) = 0;
};

}

#endif
