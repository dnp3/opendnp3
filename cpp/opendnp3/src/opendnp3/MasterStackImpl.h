#ifndef __MASTER_STACK_IMPL_H_
#define __MASTER_STACK_IMPL_H_

#include <opendnp3/IMaster.h>
#include <opendnp3/MasterStackConfig.h>
#include <openpal/Visibility.h>

#include "Master.h"
#include "ApplicationStack.h"


namespace opendnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class DLL_LOCAL MasterStackImpl : public IMaster
{
public:

	MasterStackImpl(
	        openpal::Logger,
	        openpal::IExecutor* apExecutor,
	        IMeasurementHandler* apPublisher,
	        IUTCTimeSource* apTimeSource,
	        AsyncTaskGroup* apTaskGroup,
	        const MasterStackConfig& arCfg,
			std::function<void (bool)> aEnableDisableFunc,
	        std::function<void (IMaster*)> aOnShutdown);

	ICommandProcessor* GetCommandProcessor();

	ILinkContext* GetLinkContext();

	void SetLinkRouter(ILinkRouter* apRouter);

	void AddStateListener(std::function<void (StackState)> aListener);

	MasterScan GetIntegrityScan();

	openpal::IExecutor* GetExecutor() { return mpExecutor; }

	MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate);

	void Shutdown();

private:
	IExecutor* mpExecutor;
	ApplicationStack mAppStack;
	Master mMaster;
	std::function<void (IMaster*)> mOnShutdown;

};

}

#endif

