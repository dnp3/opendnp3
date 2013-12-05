#ifndef __IMEASUREMENT_UPDATE_H_
#define __IMEASUREMENT_UPDATE_H_

#include "DataTypes.h"
#include "OctetString.h"

#include <vector>

#include "IDataObserver.h"

namespace opendnp3 
{
  
  template <class T>
  class IndexedValue
  {
	public:
		IndexedValue(const T& arValue, uint32_t aIndex) : value(arValue), index(aIndex)
		{}

		T value;
		uint32_t index;
  };

  class IMeasurementUpdate
  {
	  public:

	  typedef std::vector<IndexedValue<Binary>> BinaryContainer;
	  typedef std::vector<IndexedValue<Analog>> AnalogContainer;
	  typedef std::vector<IndexedValue<Counter>> CounterContainer;
	  typedef std::vector<IndexedValue<ControlStatus>> ControlStatusContainer;
	  typedef std::vector<IndexedValue<SetpointStatus>> SetpointStatusContainer;
	  typedef std::vector<IndexedValue<OctetString>> OctetStringContainer;
	
	  virtual const BinaryContainer& BinaryUpdates() const = 0;
	  virtual const AnalogContainer& AnalogUpdates() const = 0;
	  virtual const CounterContainer& CounterUpdates() const = 0;
	  virtual const ControlStatusContainer& ControlStatusUpdates() const = 0;
	  virtual const SetpointStatusContainer& SetpointStatusUpdates() const = 0;
	  virtual const OctetStringContainer& OctetStringUpdates() const = 0;

	  // TODO - remove this function, it's only a temporary adapter
	  void Apply(IDataObserver* apObserver) const
	  {
		Transaction t(apObserver);
		for(auto b: this->BinaryUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->AnalogUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->CounterUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->ControlStatusUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->SetpointStatusUpdates()) apObserver->Update(b.value, b.index);	
	  }
		
  };

}

#endif
