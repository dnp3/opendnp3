#include "Timeout.h"

using namespace asiopal;
using namespace std::chrono;

namespace opendnp3
{

Timeout :: Timeout(timer_clock::duration aTimeout)
	: mExpireTime(timer_clock::now() + aTimeout)
{

}

bool Timeout :: IsExpired()
{
	return timer_clock::now() >= mExpireTime;
}

timer_clock::duration Timeout :: Remaining()
{
	return mExpireTime - timer_clock::now();
}

}
