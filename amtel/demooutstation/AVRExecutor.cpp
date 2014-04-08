
#include "AVRExecutor.h"

using namespace openpal;

MonotonicTimestamp AVRExecutor::GetTime()
{
	return MonotonicTimestamp::Min();	
}

ITimer* AVRExecutor::Start(const TimeDuration& arDuration, const Runnable& runnable)
{
	return AVRTimer::Inst();
}
	
ITimer* AVRExecutor::Start(const MonotonicTimestamp&, const Runnable& runnable)
{
	return AVRTimer::Inst();	
}
	
void AVRExecutor::Post(const Runnable& runnable)
{	
	assert(!work.IsFull());
	work.Enqueue(runnable);
}

void AVRExecutor::RunOne()
{		
	if(work.IsNotEmpty())
	{
		work.Peek().Run();
		work.Pop();
	}	
}
			
void AVRExecutor::Pause()
{}
	
void AVRExecutor::Resume()
{}

AVRTimer AVRTimer::instance;

openpal::ITimer* AVRTimer::Inst()
{
	return &instance;
}

void AVRTimer::Cancel()
{

}

MonotonicTimestamp AVRTimer::ExpiresAt()
{
	return MonotonicTimestamp::Min();
}
