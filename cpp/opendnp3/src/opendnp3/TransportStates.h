#ifndef __TRANSPORT_STATES_H_
#define __TRANSPORT_STATES_H_

#include "TLS_Base.h"

#include <opendnp3/Singleton.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

/** Represents the closed state, only come online
*/
class DLL_LOCAL TLS_Closed : public TLS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(TLS_Closed);

	void LowerLayerUp(TransportLayer*);
};

/** Represents the ready state
*/
class DLL_LOCAL TLS_Ready : public TLS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(TLS_Ready);

	void Send(const openpal::ReadOnlyBuffer&, TransportLayer*);
	void HandleReceive(const openpal::ReadOnlyBuffer&, TransportLayer*);
	void LowerLayerDown(TransportLayer*);
};

/** Represents the sending state
*/
class DLL_LOCAL TLS_Sending : public TLS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(TLS_Sending);

	void HandleReceive(const openpal::ReadOnlyBuffer&, TransportLayer*);
	void HandleSendSuccess(TransportLayer*);
	void HandleSendFailure(TransportLayer*);
	void LowerLayerDown(TransportLayer*);
};

} //end namepsace

#endif

