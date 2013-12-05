#ifndef __MASTER_STATES_H_
#define __MASTER_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <opendnp3/ObjectInterfaces.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

class ITaskCompletion;
class ITask;
class Master;
class MasterTaskBase;
class APDU;

class DLL_LOCAL AMS_Base
{
public:

	// called when a new task should be started
	virtual void StartTask(Master* c, ITask*, MasterTaskBase*);

	/* Events from application layer */

	virtual void OnLowerLayerUp(Master*);
	virtual void OnLowerLayerDown(Master*);

	virtual void OnSendSuccess(Master*);
	virtual void OnFailure(Master*);

	virtual void OnPartialResponse(Master*, const APDU&);
	virtual void OnFinalResponse(Master*, const APDU&);

	virtual void OnUnsolResponse(Master*, const APDU&);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string Name() const = 0;
#endif

protected:

	void ChangeState(Master*, AMS_Base*);
	void ChangeTask(Master*, MasterTaskBase*);
};

/* AMS_Closed */

class DLL_LOCAL AMS_Closed : public AMS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Closed);

	void OnLowerLayerUp(Master*);
};

/* AMS_OpenBase */

class DLL_LOCAL AMS_OpenBase : public AMS_Base
{
public:
	void OnUnsolResponse(Master*, const APDU&);
	virtual void OnLowerLayerDown(Master* c);
};

/* AMS_Idle */

// The only state from which a task can be started
class DLL_LOCAL AMS_Idle : public AMS_OpenBase
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Idle);

	void StartTask(Master* c, ITask*, MasterTaskBase*);
};

/* AMS_Waiting */

// Wait for responses or failure. Manipulates
// the current task instance.
class DLL_LOCAL AMS_Waiting : public AMS_OpenBase
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Waiting);

	void OnFailure(Master*);
	void OnPartialResponse(Master*, const APDU&);
	void OnFinalResponse(Master*, const APDU&);

	void OnLowerLayerDown(Master* c);
};

} //ens ns

#endif
