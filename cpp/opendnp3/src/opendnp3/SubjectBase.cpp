#include <opendnp3/SubjectBase.h>

#include <openpal/IExecutor.h>

namespace opendnp3
{

SubjectBase::SubjectBase()
{

}

void SubjectBase::AddObserver(std::function<void ()> aCallback)
{
	std::lock_guard<std::mutex> lock(mSubjectMutex);
	mObservers.push_back(aCallback);
}

void SubjectBase::AddObserver(openpal::IExecutor* apExecutor, std::function<void ()> aCallback)
{
	this->AddObserver([apExecutor, aCallback]() {
		apExecutor->Post(aCallback);
	});
}

void SubjectBase::NotifyObservers()
{
	std::lock_guard<std::mutex> lock(mSubjectMutex);
for(auto obs: mObservers) obs();
}





}

