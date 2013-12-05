#include <openpal/IHandlerAsync.h>

namespace openpal
{

IHandlerAsync::IHandlerAsync(openpal::Logger& arLogger) :
	Loggable(arLogger),
	IUpperLayer(arLogger)
{

}

// For consistency sake, use NVII pattern in case we want pre/post conditions in the future
void IHandlerAsync::OnOpenFailure()
{
	this->_OnOpenFailure();
}

}
