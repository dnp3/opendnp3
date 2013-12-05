#ifndef __TEST_TYPEDEFS_H_
#define __TEST_TYPEDEFS_H_

#include <openpal/TimeDuration.h>
#include <functional>

namespace opendnp3
{

class FlexibleDataObserver;
const auto G_TEST_TIMEOUT = openpal::TimeDuration::Seconds(60);

typedef std::function<bool ()> EvalFunc;
typedef std::function<bool (FlexibleDataObserver*)> OutputEvalFunc;

}

#endif

