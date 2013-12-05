#ifndef __TIMER_CLOCK_TYPE_H_
#define __TIMER_CLOCK_TYPE_H_

#include <chrono>

namespace asiopal
{

#ifndef SIZE_MAX
#define SIZE_MAX ~0
#endif

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if defined(__GNUC__) && (GCC_VERSION < 40700)
typedef std::chrono::monotonic_clock timer_clock;
#else
typedef std::chrono::steady_clock timer_clock;
#endif

} //end namespace

#endif

