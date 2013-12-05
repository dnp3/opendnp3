#include "StackBase.h"

#include <openpal/IExecutor.h>

namespace opendnp3
{

StackBase::StackBase(openpal::IExecutor* apExecutor) : mpExecutor(apExecutor)
{}

void StackBase::AddStateListener(std::function<void (StackState)> aCallback)
{	
	mListeners.push_back(aCallback);
	Queue(aCallback, this->GetState());
}

void StackBase::Queue(const std::function<void (StackState)>& arFunc, StackState aState)
{
	mpExecutor->Post([=]() { arFunc(aState); });
}

void StackBase::NotifyListeners(StackState aState)
{
	for(auto callback : mListeners) Queue(callback, aState);	
}

}

