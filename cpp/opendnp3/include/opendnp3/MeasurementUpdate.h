#ifndef __MEASUREMENT_UPDATE_H_
#define __MEASUREMENT_UPDATE_H_

#include "IMeasurementUpdate.h"

#include <vector>

namespace opendnp3 
{  
  
  class MeasurementUpdate : public IMeasurementUpdate
  {
	public:
		MeasurementUpdate();

		MeasurementUpdate(MeasurementUpdate&& arOther);

		const BinaryContainer& BinaryUpdates() const override;
		const AnalogContainer& AnalogUpdates() const override;
	    const CounterContainer& CounterUpdates() const override;
	    const ControlStatusContainer& ControlStatusUpdates() const override;
	    const SetpointStatusContainer& SetpointStatusUpdates() const override;
		const OctetStringContainer& OctetStringUpdates() const override;

		bool HasUpdates();

		// functions for building an update
		void Add(const Binary& arValue, uint32_t aIndex);
		void Add(const Analog& arValue, uint32_t aIndex);
		void Add(const Counter& arValue, uint32_t aIndex);
		void Add(const ControlStatus& arValue, uint32_t aIndex);
		void Add(const SetpointStatus& arValue, uint32_t aIndex);
		void Add(const OctetString& arValue, uint32_t aIndex);
		
	private:
		std::vector<IndexedValue<Binary>> mBinaryVec;
		std::vector<IndexedValue<Analog>> mAnalogVec;
		std::vector<IndexedValue<Counter>> mCounterVec;
		std::vector<IndexedValue<ControlStatus>> mControlStatusVec;
		std::vector<IndexedValue<SetpointStatus>> mSetpointStatusVec;		
		std::vector<IndexedValue<OctetString>> mOctetStringVec;

		MeasurementUpdate(const MeasurementUpdate& that);
		MeasurementUpdate& operator=(const MeasurementUpdate& that);
  };

}

#endif
