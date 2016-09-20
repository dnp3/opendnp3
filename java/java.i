/* File: example.i */
%module opendnp3
%{

%}

// automatic type-maps for integer types
%include "stdint.i"

// set up all the enums using java enums
%include "enums.swg"
%include "opendnp3/gen/DoubleBit.h"
%include "opendnp3/gen/IntervalUnits.h"
%include "opendnp3/gen/BinaryQuality.h"
%include "opendnp3/gen/AnalogQuality.h"
%include "opendnp3/gen/CounterQuality.h"
%include "opendnp3/gen/FrozenCounterQuality.h"
%include "opendnp3/gen/BinaryOutputStatusQuality.h"
%include "opendnp3/gen/AnalogOutputStatusQuality.h"


// rename all methods to lowercamelcase
// %rename("%(lowercamelcase)s") "";

// include classes
%include "opendnp3/app/Flags.h"
%include "openpal/serialization/UInt48Type.h"
%include "opendnp3/Types.h"
%include "opendnp3/app/BaseMeasurementTypes.h"

%template(BoolTypedMeasurement) opendnp3::TypedMeasurement<bool>;
%template(DoubleBitTypedMeasurement) opendnp3::TypedMeasurement<DoubleBit>;
%template(DoubleTypedMeasurement) opendnp3::TypedMeasurement<double>;
%template(UInt32TypedMeasurement) opendnp3::TypedMeasurement<uint32_t>;

%include "opendnp3/app/MeasurementTypes.h"