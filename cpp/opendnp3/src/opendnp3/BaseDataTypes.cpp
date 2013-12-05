#include <opendnp3/BaseDataTypes.h>

using namespace std;

namespace opendnp3
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string GetDataTypeName(DataTypes aType)
{
	switch(aType) {
	case(DT_BINARY):
		return "Binary";
	case(DT_ANALOG):
		return "Analog";
	case(DT_COUNTER):
		return "Counter";
	case(DT_CONTROL_STATUS):
		return "ControlStatus";
	case(DT_SETPOINT_STATUS):
		return "SetpointStatus";
	default:
		return "Unknown";
	}
}
#endif


// DataPoint


DataPoint::DataPoint(uint8_t aQuality, DataTypes aType) :
	mQuality(aQuality), mTime(0),
	mType(aType)
{}

// BoolDataPoint


BoolDataPoint::BoolDataPoint(uint8_t aQuality, DataTypes aType, uint8_t aValueMask) :
	DataPoint(aQuality, aType),
	mValueMask(aValueMask)
{}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string BoolDataPoint::ToString() const
{
	std::ostringstream oss;
	oss << "Value: " << (GetValue() ? "true" : "false") << " Quality: " << static_cast<int>(GetQuality());
	return oss.str();
}
#endif

template<>
bool ExceedsDeadband<double>(const double& val1, const double& val2, double aDeadband)
{
	double val = fabs(val1 - val2);

	if(val == std::numeric_limits<double>::infinity()) return true;
	else {
		return fabs(val) > aDeadband;
	}

}
}

