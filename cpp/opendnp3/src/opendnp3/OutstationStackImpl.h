#ifndef __OUTSTATION_STACK_IMPL_H_
#define __OUTSTATION_STACK_IMPL_H_

#include <opendnp3/IOutstation.h>
#include <opendnp3/SlaveStackConfig.h>
#include <openpal/Location.h>

#include "Slave.h"
#include "ApplicationStack.h"


namespace opendnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class DLL_LOCAL OutstationStackImpl : public IOutstation
{
public:

	OutstationStackImpl(
	        openpal::Logger&,
	        openpal::IExecutor* apExecutor,
	        ITimeWriteHandler* apTimeWriteHandler,
	        ICommandHandler* apCmdHandler,
	        const SlaveStackConfig& arCfg,
			std::function<void (bool)> aEnableDisableFunc,
	        std::function<void (IOutstation*)> aOnShutdown);

	IDataObserver* GetDataObserver();

	void SetNeedTimeIIN();

	ILinkContext* GetLinkContext();

	void SetLinkRouter(ILinkRouter* apRouter);

	openpal::IExecutor* GetExecutor() { return mpExecutor; }

	void AddStateListener(std::function<void (StackState)> aListener);

	void Shutdown();

private:
	openpal::IExecutor* mpExecutor;
	ApplicationStack mAppStack;
	Database mDB;
	Slave mSlave;
	std::function<void (IOutstation*)> mOnShutdown;
};

}

#endif

