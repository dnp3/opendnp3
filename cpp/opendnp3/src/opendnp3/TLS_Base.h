#ifndef __TLS_BASE_H_
#define __TLS_BASE_H_

#include <opendnp3/Uncopyable.h>
#include <openpal/Visibility.h>
#include <openpal/BufferWrapper.h>

#include <string>
#include <cstdint>

namespace opendnp3
{

class TransportLayer;


/**
Base class for all TransportLayerStates (TLS)
*/
class DLL_LOCAL TLS_Base
{
public:
	virtual void Send(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer*);
	virtual void HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer*);

	// TPDU failure/success handlers
	virtual void HandleSendSuccess(TransportLayer*);
	virtual void HandleSendFailure(TransportLayer*);

	virtual void LowerLayerUp(TransportLayer*);
	virtual void LowerLayerDown(TransportLayer*);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string Name() const = 0;
#endif
};

}

#endif

