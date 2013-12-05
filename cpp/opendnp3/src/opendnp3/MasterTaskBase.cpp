#include "MasterTaskBase.h"

#include "APDU.h"
#include "AsyncTaskInterfaces.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

MasterTaskBase::MasterTaskBase(openpal::Logger& arLogger) :
	Loggable(arLogger)
{}

TaskResult MasterTaskBase::OnPartialResponse(const APDU& arAPDU)
{
	if(this->ValidateIIN(arAPDU.GetIIN())) return this->_OnPartialResponse(arAPDU);
	else return TR_FAIL;
}

TaskResult MasterTaskBase::OnFinalResponse(const APDU& arAPDU)
{
	if(this->ValidateIIN(arAPDU.GetIIN())) return this->_OnFinalResponse(arAPDU);
	else return TR_FAIL;
}

bool MasterTaskBase::ValidateIIN(const IINField& GetIIN) const
{
	return true;
}

SingleRspBase::SingleRspBase(openpal::Logger& arLogger) : MasterTaskBase(arLogger)
{}

TaskResult SingleRspBase::_OnPartialResponse(const APDU&)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring non-FIN response to task: " << this->Name());
	return TR_FAIL;
}

SimpleRspBase::SimpleRspBase(openpal::Logger& arLogger) : SingleRspBase(arLogger)
{}

TaskResult SimpleRspBase::_OnFinalResponse(const APDU& arAPDU)
{
	if(arAPDU.BeginRead().Count() > 0) {
		LOG_BLOCK(LogLevel::Warning, "Unexpected object headers in response: " << this->Name());
	}

	return TR_SUCCESS;
}

} //ens ns

