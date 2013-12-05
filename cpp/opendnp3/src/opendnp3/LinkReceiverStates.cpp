#include "LinkReceiverStates.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>

#include "LinkLayerReceiver.h"
#include "DNPCrc.h"

namespace opendnp3
{

////////////////////////////////////
// Base class
////////////////////////////////////

bool LRS_Base::Parse(LinkLayerReceiver*)
{
	MACRO_THROW_EXCEPTION(openpal::InvalidStateException, this->Name());
}

////////////////////////////////////
// Inherited States
////////////////////////////////////

LRS_Sync LRS_Sync::mInstance;

bool LRS_Sync::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < 2) return false;
	else {
		if(c->Sync0564()) c->ChangeState(LRS_Header::Inst());
		return true;
	}
}

LRS_Header LRS_Header::mInstance;

bool LRS_Header::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < 10) return false;
	else {
		if(c->ReadHeader()) {
			c->ChangeState(LRS_Body::Inst());
		}
		else {
			c->FailFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		return true;
	}
}

LRS_Body LRS_Body::mInstance;

bool LRS_Body::Parse(LinkLayerReceiver* c)
{
	if(c->NumReadBytes() < c->mFrameSize) return false;
	else {
		if(c->ValidateBody()) {
			c->PushFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		else {
			c->FailFrame();
			c->ChangeState(LRS_Sync::Inst());
		}
		return true;
	}
}

}

