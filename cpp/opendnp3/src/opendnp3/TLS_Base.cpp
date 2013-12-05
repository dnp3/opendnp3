#include "TLS_Base.h"

#include "TransportLayer.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>

using namespace openpal;

namespace opendnp3
{

void TLS_Base::Send(const ReadOnlyBuffer&, TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

void TLS_Base::HandleReceive(const ReadOnlyBuffer&, TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

void TLS_Base::HandleSendSuccess(TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

void TLS_Base::HandleSendFailure(TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

void TLS_Base::LowerLayerUp(TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

void TLS_Base::LowerLayerDown(TransportLayer*)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name() );
}

}

