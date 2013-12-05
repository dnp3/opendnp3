#ifndef _TIMEOUT_H__
#define _TIMEOUT_H__

#include <chrono>
#include <asiopal/TimerClockType.h>

namespace opendnp3
{

// Use this class to simplify writing do loops with a timeout
// it minimizes the number of calls to get datetime and allows
// us to easily replace the implementation later if we find an
// even more effecient way to implement the timeout checking.
//
// Intended Usage:
//
// Timeout to(5000);
// do{
//	  //call some subordinate slow function
//	  bool success = WaitForInput(to.Remaining());
//
//		//do something on success
//		if(success) return or break;
//
//		//or go back around the loop, the next call to
//		//remaining will be guarnteed to be > 0
// }while(!to.IsExpired());
class Timeout
{
public:
	// constuctor, timeout will expire this many mills in the future
	Timeout(asiopal::timer_clock::duration aTimeout);

	// returns whether its expired
	bool IsExpired();

	// returns how much time is left
	asiopal::timer_clock::duration Remaining();


private:

	asiopal::timer_clock::time_point mExpireTime;

};


}

#endif
