#include "AppInterfaces.h"

#include <openpal/Location.h>
#include <openpal/Exception.h>

using namespace openpal;

namespace opendnp3
{

void IAppUser::OnPartialResponse(const APDU&)
{
	MACRO_THROW_EXCEPTION(Exception, "Unhandled frame");
}

void IAppUser::OnFinalResponse(const APDU&)
{
	MACRO_THROW_EXCEPTION(Exception, "Unhandled frame");
}

void IAppUser::OnUnsolResponse(const APDU&)
{
	MACRO_THROW_EXCEPTION(Exception, "Unhandled frame");
}

void IAppUser::OnRequest(const APDU&, SequenceInfo)
{
	MACRO_THROW_EXCEPTION(Exception, "Unhandled frame");
}


void IAppUser::OnUnknownObject()
{
	MACRO_THROW_EXCEPTION(Exception, "Unhandled frame");
}

} //end ns
