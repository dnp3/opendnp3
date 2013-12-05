#ifndef __SEC_LINK_LAYER_STATES_H_
#define __SEC_LINK_LAYER_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <openpal/Visibility.h>

#include "LinkLayer.h"

namespace opendnp3
{

class DLL_LOCAL SecStateBase
{
public:

	/* Incoming messages to secondary station */

	void ResetLinkStates(LinkLayer*);
	void RequestLinkStatus(LinkLayer*);
	void UnconfirmedUserData(LinkLayer*, const openpal::ReadOnlyBuffer&);

	virtual void TestLinkStatus(LinkLayer*, bool aFcb) = 0;
	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&) = 0;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	//every concrete state implements this for logging purposes
	virtual std::string Name() const = 0;
#endif
};

////////////////////////////////////////////////////////
//	Class SLLS_UnReset
////////////////////////////////////////////////////////
class DLL_LOCAL SLLS_NotReset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_NotReset);

	void TestLinkStatus(LinkLayer*, bool aFcb);
	void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&);
};

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
class DLL_LOCAL SLLS_Reset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_Reset);

	void TestLinkStatus(LinkLayer*, bool aFcb);
	void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&);
};

} //end namepsace

#endif

