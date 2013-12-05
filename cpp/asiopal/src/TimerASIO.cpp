#include "TimerASIO.h"

using namespace openpal;

namespace asiopal
{

TimerASIO::TimerASIO(boost::asio::strand* apStrand) :
	mCanceled(false),
	mTimer(apStrand->get_io_service())
{

}

/**
 * Return the timer's expiry time as an absolute time.
 */
openpal::MonotonicTimestamp TimerASIO::ExpiresAt()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(mTimer.expires_at().time_since_epoch()).count();
}

void TimerASIO::Cancel()
{
	assert(!mCanceled);
	mTimer.cancel();
	mCanceled = true;
}


}
