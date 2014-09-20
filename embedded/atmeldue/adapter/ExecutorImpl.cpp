
#include "ExecutorImpl.h"

#include "TimerImpl.h"

using namespace openpal;

void ExecutorImpl::Tick()
{
	++ticks;	
}

ExecutorImpl::ExecutorImpl(uint8_t maxQueueSize, uint8_t maxtimers) : 
	ticks(0), 	
	work(maxQueueSize), 
	idleTimers(maxtimers)
{	
	for(uint8_t i = 0; i < timers.Size(); ++i)
	{
		idleTimers.Enqueue(&timers[i]);
	}
}

MonotonicTimestamp ExecutorImpl::GetTime()
{		
	return MonotonicTimestamp(ticks); // every tick represents 1 milliseconds
}

ITimer* ExecutorImpl::Start(const TimeDuration& duration, const Action0& action)
{	
	return Start(GetTime().Add(duration), action);
}
	
ITimer* ExecutorImpl::Start(const MonotonicTimestamp& ts, const Action0& action)
{	
	assert(idleTimers.IsNotEmpty());
	TimerImpl** pTimer = idleTimers.Pop();
	(*pTimer)->Set(this, action, ts);
	this->activeTimers.Add(*pTimer);
	return *pTimer;
}
	
void ExecutorImpl::Post(const Action0& action)
{		
	assert(!work.IsFull());
	work.Enqueue(action);
}

void ExecutorImpl::Run()
{
	while(RunOne());
}

bool ExecutorImpl::RunOne()
{				
	if(RunOneTimer())
	{
		return true;
	}
	else
	{
		if(work.IsNotEmpty())
		{
			work.Peek()->Apply();
			work.Pop();
			return true;
		}
		else
		{
			return false;
		}
	}	
}

bool ExecutorImpl::RunOneTimer()
{	
	MonotonicTimestamp time = GetTime();
	auto expired = [time](TimerImpl* pTimer) { return pTimer->ExpiresAt().milliseconds < time.milliseconds; };
	auto pNode = activeTimers.RemoveFirst(expired);
	if(pNode)
	{
		idleTimers.Enqueue(pNode->value);
		
		// make a copy of the task and run it
		Action0 copy(pNode->value->action);
		copy.Apply();		
		return true;
	}
	else
	{
		return false;
	}
}
			
void ExecutorImpl::OnCancel(TimerImpl* pTimer)
{
	auto matches = [pTimer](TimerImpl* pItem){ return pTimer == pItem; };
	activeTimers.RemoveFirst(matches);	
	idleTimers.Enqueue(pTimer);
}
