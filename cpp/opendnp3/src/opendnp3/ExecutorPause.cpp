#include "ExecutorPause.h"

#include <openpal/IExecutor.h>

using namespace openpal;

namespace opendnp3
{

ExecutorPause::ExecutorPause(IExecutor* apExecutor) :
	mpExecutor(apExecutor),
	mPaused(false),
	mComplete(false),
	mExit(false)
{
	mpExecutor->Post([this]() {
		this->Pause();
	});
	std::unique_lock<std::mutex> lock(mMutex);
	while(!mPaused) mCondition.wait(lock);
}

ExecutorPause::~ExecutorPause()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mComplete = true;
	mCondition.notify_one();
	while(!mExit) mCondition.wait(lock);
}

void ExecutorPause::Pause()
{
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mPaused = true;
		mCondition.notify_one();
		while(!mComplete) mCondition.wait(lock);
	}
	mExit = true; // make sure we're not going to touch from other thread after destruction
	mCondition.notify_one();
}

}


