#ifndef __FUNCTION_CODE_HELPERS_H_
#define __FUNCTION_CODE_HELPERS_H_

#include "gen/FunctionCode.h"

namespace opendnp3
{

/// @return true if the function is a response, false otherwise
bool IsResponse(FunctionCode aCode);

/// @return true if the function is a request, false otherwise
bool IsRequest(FunctionCode aCode);

}

#endif
