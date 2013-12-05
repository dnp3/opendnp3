#ifndef __STACK_BASE_H_
#define __STACK_BASE_H_

#include <openpal/Visibility.h>
#include <opendnp3/gen/StackState.h>

#include <functional>
#include <vector>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

class DLL_LOCAL StackBase
{
public:
	StackBase(openpal::IExecutor* apExecutor);

	void AddStateListener(std::function<void (StackState)> aCallback);

protected:
	void NotifyListeners(StackState aState);

	void Queue(const std::function<void (StackState)>& arFunc, StackState aState);

	// implement in inherited class
	virtual StackState GetState() = 0;

	openpal::IExecutor* mpExecutor;

private:
	std::vector<std::function<void (StackState)>> mListeners;
};
}

#endif
