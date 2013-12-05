#include <opendnp3/IMeasurementHandler.h>

namespace opendnp3
{

NullMeasurementHandler NullMeasurementHandler::msInstance;

PrintingMeasurementHandler PrintingMeasurementHandler::msInstance;

void PrintingMeasurementHandler::Print(const IMeasurementUpdate& arUpdate)
{
	for(auto pair: arUpdate.BinaryUpdates()) PrintAny(pair.value, pair.index);
	for(auto pair: arUpdate.AnalogUpdates()) PrintAny(pair.value, pair.index);
	for(auto pair: arUpdate.CounterUpdates()) PrintAny(pair.value, pair.index);
	for(auto pair: arUpdate.ControlStatusUpdates()) PrintAny(pair.value, pair.index);
	for(auto pair: arUpdate.SetpointStatusUpdates()) PrintAny(pair.value, pair.index);
}

}
