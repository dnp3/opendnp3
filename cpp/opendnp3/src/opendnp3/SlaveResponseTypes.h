#ifndef __SLAVE_RESPONSE_TYPES_H_
#define __SLAVE_RESPONSE_TYPES_H_

#include <opendnp3/ObjectInterfaces.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/SlaveConfig.h>

#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * Reads a slave config object and and translates the configuration to
 * singletons.
 */
class DLL_LOCAL SlaveResponseTypes
{
public:

	SlaveResponseTypes(const SlaveConfig& arCfg);

	StreamObject<Binary>* mpStaticBinary;
	StreamObject<Analog>* mpStaticAnalog;
	StreamObject<Counter>* mpStaticCounter;
	StreamObject<ControlStatus>* mpStaticControlStatus;
	StreamObject<SetpointStatus>* mpStaticSetpointStatus;

	StreamObject<Binary>* mpEventBinary;
	StreamObject<Analog>* mpEventAnalog;
	StreamObject<Counter>* mpEventCounter;

	SizeByVariationObject* mpEventVto;

private:

	static StreamObject<Binary>* GetStaticBinary(StaticBinaryResponse);
	static StreamObject<Analog>* GetStaticAnalog(StaticAnalogResponse);
	static StreamObject<Counter>* GetStaticCounter(StaticCounterResponse);
	static StreamObject<SetpointStatus>* GetStaticSetpointStatus(StaticSetpointStatusResponse);

	static StreamObject<Binary>* GetEventBinary(EventBinaryResponse);
	static StreamObject<Analog>* GetEventAnalog(EventAnalogResponse);
	static StreamObject<Counter>* GetEventCounter(EventCounterResponse);

};

}

#endif

