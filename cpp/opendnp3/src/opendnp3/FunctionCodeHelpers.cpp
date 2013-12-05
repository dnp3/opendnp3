#include "FunctionCodeHelpers.h"

namespace opendnp3
{

bool IsResponse(FunctionCode aCode)
{
	return (aCode == FunctionCode::RESPONSE) || (aCode == FunctionCode::UNSOLICITED_RESPONSE);
}

bool IsRequest(FunctionCode aCode)
{
	switch(aCode) {
	case(FunctionCode::CONFIRM):
	case(FunctionCode::RESPONSE):
	case(FunctionCode::UNSOLICITED_RESPONSE):
		return false;
	default:
		return true;
	}
}

}
