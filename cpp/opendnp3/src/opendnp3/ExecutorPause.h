#ifndef __EXECUTOR_PAUSE_H_
#define __EXECUTOR_PAUSE_H_

#include <openpal/Visibility.h>

#include <mutex>
#include <condition_variable>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

class DLL_LOCAL ExecutorPause
{
public:
	ExecutorPause(openpal::IExecutor* apExecutor);
	~ExecutorPause();

private:

	void Pause();

	std::mutex mMutex;
	std::condition_variable mCondition;
	openpal::IExecutor* mpExecutor;
	bool mPaused;
	bool mComplete;
	bool mExit;
};

}

#endif
