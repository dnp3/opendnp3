#ifndef __DNP3_CHANNEL_H_
#define __DNP3_CHANNEL_H_

#include <opendnp3/IChannel.h>
#include <opendnp3/SlaveStackConfig.h>

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>

#include "LinkLayerRouter.h"

#ifndef OPENDNP3_NO_MASTER
#include "AsyncTaskGroup.h"
#endif

#include <memory>
#include <functional>

namespace openpal
{
class IPhysicalLayerAsync;
}


namespace opendnp3
{

class IStack;
class IOutstation;
class ICommandHandler;
class ITimeWriteHandler;
class IMeasurementHandler;

class DLL_LOCAL DNP3Channel: public IChannel, private openpal::Loggable
{
public:
	DNP3Channel(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, IPhysicalLayerAsync* apPhys, std::function<void (DNP3Channel*)> aOnShutdown);
	~DNP3Channel();

	// Implement IChannel - these are exposed to clients

	void Shutdown();

	void AddStateListener(std::function<void (ChannelState)> aListener);

	openpal::IExecutor* GetExecutor();

#ifndef OPENDNP3_NO_MASTER

	IMaster* AddMaster(		const std::string& arLoggerId,
	                                LogLevel aLevel,
	                                IMeasurementHandler* apPublisher,
	                                openpal::IUTCTimeSource* apTimeSource,
	                                const MasterStackConfig& arCfg);

#endif

	IOutstation* AddOutstation(	const std::string& arLoggerId,
	                                LogLevel aLevel,
	                                ICommandHandler* apCmdHandler,
	                                ITimeWriteHandler* apTimeWriteHandler,
	                                const SlaveStackConfig&);

	// Helper functions only available inside DNP3Manager

private:

	std::function<void (bool)> GetEnableDisableRoute(IExecutor*, LinkLayerRouter*, LinkRoute);
	

	void Cleanup();

	void OnStackShutdown(IStack* apStack, LinkRoute aRoute);
	std::auto_ptr<IPhysicalLayerAsync> mpPhys;
	std::function<void (DNP3Channel*)> mOnShutdown;
	LinkLayerRouter mRouter;

#ifndef OPENDNP3_NO_MASTER
	AsyncTaskGroup mGroup;
#endif

	std::set<IStack*> mStacks;

};

}

#endif

