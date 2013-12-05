#include <asiopal/SerialTypes.h>

namespace asiopal
{


ParityType GetParityFromInt(int parity)
{
	switch(parity) {
	case (1): return PAR_EVEN;
	case (2): return PAR_ODD;
	default: return PAR_NONE;
	}
}

FlowType GetFlowTypeFromInt(int flowControl)
{
	switch(flowControl) {
	case (1): return FLOW_HARDWARE;
	case (2): return FLOW_XONXOFF;
	default: return FLOW_NONE;
	}
}

}



