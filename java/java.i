/* File: example.i */
%module opendnp3
%{

%}

// set up all the enums using java enums
%include "enums.swg"
%include "opendnp3/gen/DoubleBit.h"
%include "opendnp3/gen/IntervalUnits.h"

// include classes
%include "opendnp3/app/BaseMeasurementTypes.h"
%template(BoolTypedMeasurement) opendnp3::TypedMeasurement<bool>;
%template(DoubleBitTypedMeasurement) opendnp3::TypedMeasurement<DoubleBit>;
%template(DoubleTypedMeasurement) opendnp3::TypedMeasurement<double>;
%template(UInt32TypedMeasurement) opendnp3::TypedMeasurement<uint32_t>;

%include "opendnp3/app/MeasurementTypes.h"