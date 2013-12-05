#include "OutstationStackImpl.h"

namespace opendnp3
{

OutstationStackImpl::OutstationStackImpl(
        openpal::Logger& arLogger,
        openpal::IExecutor* apExecutor,
        ITimeWriteHandler* apTimeWriteHandler,
        ICommandHandler* apCmdHandler,
        const SlaveStackConfig& arCfg,
		std::function<void (bool)> aEnableDisableFunc,
        std::function<void (IOutstation*)> aOnShutdown) :
	IOutstation(arLogger, aEnableDisableFunc),
	mpExecutor(apExecutor),
	mAppStack(arLogger, apExecutor, arCfg.app, arCfg.link),
	mDB(arLogger),
	mSlave(arLogger.GetSubLogger("outstation"), &mAppStack.mApplication, apExecutor, apTimeWriteHandler, &mDB, apCmdHandler, arCfg.slave),
	mOnShutdown(aOnShutdown)
{
	mAppStack.mApplication.SetUser(&mSlave);
	mDB.Configure(arCfg.device);
}

IDataObserver* OutstationStackImpl::GetDataObserver()
{
	return mSlave.GetDataObserver();
}

void OutstationStackImpl::SetNeedTimeIIN()
{
	mpExecutor->Post([this](){ this->mSlave.SetNeedTimeIIN(); });
}

ILinkContext* OutstationStackImpl::GetLinkContext()
{
	return &mAppStack.mLink;
}

void OutstationStackImpl::SetLinkRouter(ILinkRouter* apRouter)
{
	mAppStack.mLink.SetRouter(apRouter);
}

void OutstationStackImpl::AddStateListener(std::function<void (StackState)> aListener)
{
	mSlave.AddStateListener(aListener);
}

void OutstationStackImpl::Shutdown()
{
	mOnShutdown(this);
}

}

