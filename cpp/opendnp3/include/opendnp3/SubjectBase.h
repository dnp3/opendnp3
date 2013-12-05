#ifndef __SUBJECT_BASE_H_
#define __SUBJECT_BASE_H_

#include <vector>
#include <mutex>
#include <functional>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

/**
* Helper class designed to be used with private inheritance. Allows observers to be notified when some
* state changes.
*/
class SubjectBase
{

public:
	SubjectBase();
	virtual ~SubjectBase() {}

	// implement the ISubject interface
	void AddObserver(std::function<void ()> aCallback);

	void AddObserver(openpal::IExecutor* apExecutor, std::function<void ()> aCallback);

protected:

	void NotifyObservers();

private:
	std::mutex mSubjectMutex;
	std::vector< std::function<void ()> > mObservers;
};

}

#endif

