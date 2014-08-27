
#include "AVRExecutor.h"

#include "AVRTimer.h"

#include "CriticalSection.h"

using namespace openpal;

AVRExecutor* gpExecutor = nullptr;

/* TODO - ARM specific timer handler
SIGNAL(TIMER1_COMPA_vect)
{
	if(gpExecutor)
	{
		gpExecutor->Tick();		
	}	
}
*/

void AVRExecutor::Tick()
{
	++ticks;	
}

void AVRExecutor::Sleep()
{
	// TODO - Define a sleep mode
}


void AVRExecutor::Init()
{		
	// TODO - enable ARM time interrupts to call the executor tick
 	
	gpExecutor = this;		
}

AVRExecutor::AVRExecutor() : ticks(0)
{	
	for(uint8_t i = 0; i < timers.Size(); ++i)
	{
		idleTimers.Enqueue(&timers[i]);
	}
}

MonotonicTimestamp AVRExecutor::GetTime()
{	
	// disable interrupts to ensure atomic access to the 'ticks' variable
	CriticalSection cs;
	return MonotonicTimestamp(ticks*10); // every tick represents 10 milliseconds since Init()				
}

ITimer* AVRExecutor::Start(const TimeDuration& duration, const Action0& action)
{	
	return Start(GetTime().Add(duration), action);
}
	
ITimer* AVRExecutor::Start(const MonotonicTimestamp& ts, const Action0& action)
{
	assert(idleTimers.IsNotEmpty());
	AVRTimer** pTimer = idleTimers.Pop();
	(*pTimer)->Set(this, action, ts);
	this->activeTimers.Add(*pTimer);
	return *pTimer;
}
	
void AVRExecutor::Post(const Action0& action)
{	
	assert(!work.IsFull());
	work.Enqueue(action);
}

void AVRExecutor::Run()
{
	while(RunOne());
}

bool AVRExecutor::RunOne()
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

bool AVRExecutor::RunOneTimer()
{	
	MonotonicTimestamp time = GetTime();
	auto expired = [time](AVRTimer* pTimer) { return pTimer->ExpiresAt().milliseconds < time.milliseconds; };
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
			
void AVRExecutor::OnCancel(AVRTimer* pTimer)
{
	auto matches = [pTimer](AVRTimer* pItem){ return pTimer == pItem; };
	activeTimers.RemoveFirst(matches);	
	idleTimers.Enqueue(pTimer);
}
