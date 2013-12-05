#ifndef __PRI_LINK_LAYER_STATES_H_
#define __PRI_LINK_LAYER_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <openpal/Location.h>

#include "LinkLayer.h"

namespace opendnp3
{

class LinkLayer;

class DLL_LOCAL PriStateBase
{
public:

	/* Incoming messages for primary station */
	virtual void Ack(LinkLayer*, bool aIsRcvBuffFull);
	virtual void Nack(LinkLayer*, bool aIsRcvBuffFull);
	virtual void LinkStatus(LinkLayer*, bool aIsRcvBuffFull);
	virtual void NotSupported (LinkLayer*, bool aIsRcvBuffFull);

	virtual void OnTimeout(LinkLayer*);

	/*Upper layer events to handle */
	virtual void SendConfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);
	virtual void SendUnconfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	//every concrete state implements this for logging purposes
	virtual std::string Name() const = 0;
#endif

};

//	@section desc Entry state for primary station
class DLL_LOCAL PLLS_SecNotReset : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecNotReset);
	void SendUnconfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);
	void SendConfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);
};

//	@section desc for reset state
class DLL_LOCAL PLLS_SecReset : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecReset);
	void SendUnconfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);
	void SendConfirmed(LinkLayer*, const openpal::ReadOnlyBuffer& arBuffer);
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class DLL_LOCAL PLLS_ResetLinkWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ResetLinkWait);

	void Ack(LinkLayer*, bool aIsRcvBuffFull);
	void Nack(LinkLayer*  apLL, bool) {
		Failure(apLL);
	}
	void LinkStatus(LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void NotSupported (LinkLayer*  apLL, bool) {
		Failure(apLL);
	}

	void OnTimeout(LinkLayer*);

private:
	void Failure(LinkLayer*);
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class DLL_LOCAL PLLS_ConfDataWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ConfDataWait);

	void Ack(LinkLayer*, bool aIsRcvBuffFull);
	void Nack(LinkLayer* apLL, bool);
	void LinkStatus(LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void NotSupported (LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void OnTimeout(LinkLayer*);

private:
	void Failure(LinkLayer*);
};

} //end namepsace

#endif

