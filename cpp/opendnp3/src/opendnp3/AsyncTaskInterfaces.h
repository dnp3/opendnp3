#ifndef __ASYNC_TASK_INTERFACES_H_
#define __ASYNC_TASK_INTERFACES_H_

#include <functional>

#include <openpal/Visibility.h>

namespace opendnp3
{

class DLL_LOCAL ITask
{
public:

	virtual void OnComplete(bool aSuccess) = 0;

	// Enable the task and notify the task group which might execute another task
	virtual void Enable() = 0;

	// Disable ''
	virtual void Disable() = 0;

	// Enable without notifying the task group
	virtual void SilentEnable() = 0;

	// Diable ''
	virtual void SilentDisable() = 0;
};

typedef std::function<void (ITask*)> TaskHandler;

}

#endif
