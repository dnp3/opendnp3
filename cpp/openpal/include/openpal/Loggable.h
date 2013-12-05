#ifndef __LOGGABLE_H_
#define __LOGGABLE_H_

#include "Logger.h"

namespace openpal
{

class Loggable
{
public:
	Loggable(Logger& arLogger);

protected:
	Logger mLogger;
};

}

#endif

