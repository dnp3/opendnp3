#include "TransportStates.h"

#include "TransportLayer.h"

namespace opendnp3
{

//////////////////////////////////////////////////////
//	TLS_Closed
//////////////////////////////////////////////////////
TLS_Closed TLS_Closed::mInstance;

void TLS_Closed::LowerLayerUp(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Ready::Inst());
	apContext->ThisLayerUp();
}

//////////////////////////////////////////////////////
//	TLS_Ready
//////////////////////////////////////////////////////
TLS_Ready TLS_Ready::mInstance;

void TLS_Ready::Send(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Sending::Inst());
	apContext->TransmitAPDU(arBuffer);
}

void TLS_Ready::LowerLayerDown(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Closed::Inst());
	apContext->ThisLayerDown();
}

void TLS_Ready::HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(arBuffer);
}

//////////////////////////////////////////////////////
//	TLS_Sending
//////////////////////////////////////////////////////
TLS_Sending TLS_Sending::mInstance;

void TLS_Sending::HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(arBuffer);
}

void TLS_Sending::HandleSendSuccess(TransportLayer* apContext)
{
	if(!apContext->ContinueSend()) {
		apContext->ChangeState(TLS_Ready::Inst()); // order important here
		apContext->SignalSendSuccess();
	}
}

void TLS_Sending::LowerLayerDown(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Closed::Inst());
	apContext->ThisLayerDown();
}

void TLS_Sending::HandleSendFailure(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Ready::Inst());
	apContext->SignalSendFailure();
}

} //end namespace

