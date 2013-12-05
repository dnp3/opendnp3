#ifndef __I_HANDLER_ASYNC_H_
#define __I_HANDLER_ASYNC_H_

#include <openpal/AsyncLayerInterfaces.h>

namespace openpal
{

class IHandlerAsync : public IUpperLayer
{
public:
	IHandlerAsync(openpal::Logger&);
	virtual ~IHandlerAsync() {}

	// In addition to all of the IUpperLayer functions, provide a mechanism to receive open failures
	// For consistency sake, use NVII pattern in case we want pre/post conditions in the future
	void OnOpenFailure();

private:

	// called when the layer didn't make a connection and has given up trying, safe to delete.
	virtual void _OnOpenFailure() = 0;
};

}

#endif
