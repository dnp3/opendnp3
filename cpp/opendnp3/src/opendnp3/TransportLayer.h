#ifndef __TRANSPORT_LAYER_H_
#define __TRANSPORT_LAYER_H_

#include "TransportRx.h"
#include "TransportTx.h"

#include <openpal/AsyncLayerInterfaces.h>

#include <opendnp3/DNPConstants.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

class TLS_Base;

/** Implements the DNP3 transport layer as a generic
asynchronous protocol stack layer
*/
class DLL_LOCAL TransportLayer : public openpal::IUpperLayer, public openpal::ILowerLayer
{
public:

	TransportLayer(openpal::Logger aLogger, size_t aFragSize = DEFAULT_FRAG_SIZE);
	virtual ~TransportLayer() {}

	/* Actions - Taken by the states/transmitter/receiver in response to events */

	void ThisLayerUp();
	void ThisLayerDown();
	void ChangeState(TLS_Base* apNewState);

	void TransmitAPDU(const openpal::ReadOnlyBuffer&);
	void TransmitTPDU(const openpal::ReadOnlyBuffer&);
	void ReceiveAPDU(const openpal::ReadOnlyBuffer&);
	void ReceiveTPDU(const openpal::ReadOnlyBuffer&);

	bool ContinueSend(); // return true if
	void SignalSendSuccess();
	void SignalSendFailure();
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/* Events - NVII delegates from ILayerUp/ILayerDown and Events produced internally */
	static std::string ToString(uint8_t aHeader);
#endif

private:

	//delegated to the states
	void _Send(const openpal::ReadOnlyBuffer&); //Implement ILowerLayer
	void _OnReceive(const openpal::ReadOnlyBuffer&); //Implement IUpperLayer

	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnLowerLayerShutdown();

	/* Members and Helpers */
	TLS_Base* mpState;

	const size_t M_FRAG_SIZE;

	/* Transmitter and Receiver Classes */
	TransportRx mReceiver;
	TransportTx mTransmitter;

	bool mThisLayerUp;
};

}

#endif

