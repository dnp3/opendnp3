#ifndef __DESTRUCTOR_HOOK_H_
#define __DESTRUCTOR_HOOK_H_

#include <vector>
#include <functional>

namespace openpal { class IExecutor; }

namespace opendnp3
{

/**
* Provides callback capabilities upon destruction. Useful for tying the lifecycle of some resource
* to the lifecycle of this object.
*/
class DestructorHook
{
public:
	DestructorHook();
	DestructorHook(openpal::IExecutor*);

	virtual ~DestructorHook();

	/**
	* Adds a destructor callback
	* @param aHook Callback that will be invoked when this class's destructor is called.
	*/
	void AddDestructorHook(std::function<void ()> aHook);

private:
	openpal::IExecutor* mpExecutor;
	std::vector<std::function<void ()>> mHooks;
};

}

#endif

