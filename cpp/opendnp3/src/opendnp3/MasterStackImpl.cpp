#include "MasterStackImpl.h"

#include <openpal/IExecutor.h>

#include "ExecutorPause.h"

using namespace openpal;

namespace opendnp3
{

MasterStackImpl::MasterStackImpl(	Logger aLogger,
                                        IExecutor* apExecutor,
                                        IMeasurementHandler* apPublisher,
                                        IUTCTimeSource* apTimeSource,
                                        AsyncTaskGroup* apTaskGroup,
                                        const MasterStackConfig& arCfg,
										std::function<void (bool)> aEnableDisableFunc,
                                        std::function<void (IMaster*)> aOnShutdown) :

	IMaster(aLogger, aEnableDisableFunc),
	mpExecutor(apExecutor),
	mAppStack(aLogger, apExecutor, arCfg.app, arCfg.link),
	mMaster(aLogger.GetSubLogger("master"), arCfg.master, &mAppStack.mApplication, apPublisher, apTaskGroup, apExecutor, apTimeSource),
	mOnShutdown(aOnShutdown)
{
	mAppStack.mApplication.SetUser(&mMaster);
}

ICommandProcessor* MasterStackImpl::GetCommandProcessor()
{
	return mMaster.GetCommandProcessor();
}

ILinkContext* MasterStackImpl::GetLinkContext()
{
	return &mAppStack.mLink;
}

void MasterStackImpl::SetLinkRouter(ILinkRouter* apRouter)
{
	mAppStack.mLink.SetRouter(apRouter);
}

MasterScan MasterStackImpl::AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate)
{
	ExecutorPause pause(mpExecutor);
	return mMaster.AddClassScan(aClassMask, aScanRate, aRetryRate);
}

MasterScan MasterStackImpl::GetIntegrityScan()
{
	return mMaster.GetIntegrityScan();
}

void MasterStackImpl::Shutdown()
{
	mOnShutdown(this);
}

void MasterStackImpl::AddStateListener(std::function<void (StackState)> aListener)
{
	mMaster.AddStateListener(aListener);
}

}

