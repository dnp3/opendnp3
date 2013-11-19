#ifndef __ASIO_DNP3_MANAGER_H_
#define __ASIO_DNP3_MANAGER_H_

#include <cstdint>
#include <functional>
#include <memory>

#include <openpal/Logger.h>
#include <openpal/TimeDuration.h>
#include <asiopal/SerialTypes.h>
#include <opendnp3/DestructorHook.h>

namespace opendnp3
{
class IChannel;
class DNP3Channel;
class DNP3Manager;
}

namespace openpal
{
class IPhysicalLayerAsync;
}

namespace asiopal
{
class EventLog;
class IOServiceThreadPool;
}


namespace asiodnp3
{

class ASIODNP3Manager : public opendnp3::DestructorHook
{
public:
	ASIODNP3Manager(
	        uint32_t aConcurrency,
	std::function<void()> aOnThreadStart = []() {},
	std::function<void()> aOnThreadExit = []() {}
	);

	~ASIODNP3Manager();

	/**
	* Add a callback to receive log messages
	* @param apLog Pointer to a callback object
	*/
	void AddLogSubscriber(openpal::ILogBase* apLog);

	openpal::ILogBase* GetLog();

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

	/**
	* Add a tcp client channel
	*
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on failure in milliseconds
	* @param arHost IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param aPort Port of remote outstation is listening on
	*/
	opendnp3::IChannel* AddTCPClient(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, const std::string& arHost, uint16_t aPort);

	/**
	* Add a tcp server channel
	*
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on bind failure in milliseconds
	* @param arEndpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param aPort Port to listen on
	*/
	opendnp3::IChannel* AddTCPServer(openpal::Logger, openpal::TimeDuration aOpenRetry, const std::string& arEndpoint, uint16_t aPort);

	/**
	* Add a serial channel
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on open failure in milliseconds
	* @param aSettings settings object that fully parameterizes the serial port
	*/
	opendnp3::IChannel* AddSerial(openpal::Logger, openpal::TimeDuration aOpenRetry, asiopal::SerialSettings aSettings);

private:

	std::unique_ptr<asiopal::EventLog> mpLog;
	std::unique_ptr<asiopal::IOServiceThreadPool> mpThreadPool;
	std::unique_ptr<opendnp3::DNP3Manager> mpManager;
};

}

#endif
