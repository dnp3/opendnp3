#include <opendnp3/DestructorHook.h>

#include <openpal/IExecutor.h>

namespace opendnp3
{

DestructorHook::DestructorHook(openpal::IExecutor* apExecutor) : mpExecutor(apExecutor)
{

}

DestructorHook::DestructorHook() : mpExecutor(nullptr)
{

}

DestructorHook::~DestructorHook()
{
	for(auto func: mHooks) {
		if(mpExecutor == nullptr) func();
		else mpExecutor->Post([func](){ func(); });
	}
}

void DestructorHook::AddDestructorHook(std::function<void ()> aHook)
{
	mHooks.push_back(aHook);
}

}
