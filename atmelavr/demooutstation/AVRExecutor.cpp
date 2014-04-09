
#include "AVRExecutor.h"

#include "AVRTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <CriticalSection.h>

using namespace openpal;

AVRExecutor* gpExecutor = nullptr;

SIGNAL(TIMER1_COMPA_vect)
{
	if(gpExecutor)
	{
		gpExecutor->Tick();		
	}	
}

void AVRExecutor::Tick()
{
	++ticks;	
}

void AVRExecutor::Sleep()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  sleep_enable();

  /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
  /*
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable(); 
  */

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  // power_all_enable();
}


void AVRExecutor::Init()
{		
	 // Configure timer 1 for CTC mode
	TCCR1B |= (1 << WGM12);

	// Set CTC compare value to ~100Hz at 16MHz AVR clock, with prescale of 1024
	OCR1A = 155;
	
	// Start timer at FCPU / 1024
	TCCR1B |= ((1 << CS10) | (1 << CS12));

	TIMSK1 |= (1 << OCIE1A); // enable output compare interrupt for A compare
	
	gpExecutor = this;	
	
	sei();
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
	//CriticalSection cs; // TODO - better way to atomic read ticks?
	return MonotonicTimestamp(ticks*10); // every tick represents 10 milliseconds since Init()				
}

ITimer* AVRExecutor::Start(const TimeDuration& duration, const Runnable& runnable)
{	
	return Start(GetTime().Add(duration), runnable);
}
	
ITimer* AVRExecutor::Start(const MonotonicTimestamp& ts, const Runnable& runnable)
{
	assert(idleTimers.IsNotEmpty());
	AVRTimer* pTimer = idleTimers.Pop();
	pTimer->Set(this, runnable, ts);
	this->activeTimers.Add(pTimer);
	return pTimer;
}
	
void AVRExecutor::Post(const Runnable& runnable)
{	
	assert(!work.IsFull());
	work.Enqueue(runnable);
}

bool AVRExecutor::RunOne()
{			
	CriticalSection cs; // TODO release interrupts before running task
	if(RunOneTimer())
	{
		return true;
	}
	else
	{
		if(work.IsNotEmpty())
		{
			work.Peek().Run();
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
		pNode->value->runnable.Run();
		idleTimers.Enqueue(pNode->value);
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
