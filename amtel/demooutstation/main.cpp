

#include <opendnp3/app/ApplicationStack.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>


#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>


#include <openpal/LogRoot.h>

using namespace opendnp3;

#include <avr/io.h> 
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BIT(x)	(1<<x)
#define SET(reg, bits) reg |= bits
#define CLEAR(reg, bits) reg &= ~bits
#define TOGGLE(reg, bits) reg ^= bits

class MockExecutor : public openpal::IExecutor
{
	public:
	
	virtual MonotonicTimestamp GetTime() override final
	{
		return MonotonicTimestamp::Min();
	}

	/** Returns a new timer based on a relative time duration */
	virtual ITimer* Start(const TimeDuration& arDuration, const Runnable& runnable) override final
	{
		assert(false);
		return nullptr;
	}

	/** Returns a new timer based on an absolute timestamp of the monotonic clock */
	virtual ITimer* Start(const MonotonicTimestamp&, const Runnable& runnable) override final
	{
		assert(false);
		return nullptr;
	}

	/** Thread-safe way to post an event to be handled asynchronously */
	virtual void Post(const Runnable& runnable) override final
	{
		
	}	
	
	protected:
	
	virtual void Pause() override final
	{
	}
	
	virtual void Resume() override final
	{
	}
	
};

int main()
{

	LogRoot root(nullptr, 0);	
	ApplicationStack stack(root.GetLogger("test"), nullptr, AppConfig(false), LinkConfig(false, false));
	
	StaticallyAllocatedDatabase<1> db;
	StaticallyAllocatedEventBuffer<1> eb;
	Database database(db.GetFacade());

	MockExecutor exe;

	Outstation outstation(
		root.GetLogger("outstation"), 
		&stack.application, 
		&exe, //executor
		NullTimeWriteHandler::Inst(),
		&database,
		eb.GetFacade(), 
		SuccessCommandHandler::Inst(), 
		OutstationConfig());


	SET(DDRB, BIT(7));		// Set PORTB, pin 7 as output
	SET(TCCR1B, BIT(CS10)|BIT(CS12));	// Set clock source, x1 

	for (;;) 
	{ 
		// Check timer value in if statement, true when count matches 1/20 of a second 
		if (TCNT1 >= 5000) 
		{ 
			TOGGLE(PORTB, BIT(7)); 	// Toggle the LED 
			TCNT1 = 0;		// Zero the timer
		} 
	} 

	return 0;
}

