#ifndef __DNP_DATABASE_TYPES_H_
#define __DNP_DATABASE_TYPES_H_

#include <opendnp3/DataTypes.h>
#include <opendnp3/PointClass.h>
#include <opendnp3/VTOData.h>

#include <openpal/Visibility.h>

#include <vector>

namespace opendnp3
{

/**
 * Common base class for point information and associated meta-data needed for
 * processing it.  The common base class allows some common operations to be
 * performed on static and event types.
 */
template<typename T>
struct DLL_LOCAL PointInfoBase {
	PointInfoBase(const T& arValue, PointClass aClass, size_t aIndex) :
		mValue(arValue),
		mClass(aClass),
		mIndex(aIndex)
	{}

	PointInfoBase() :
		mClass(PC_CLASS_0),
		mIndex(0)
	{}

	T mValue;			// current measurment (i.e. Binary, Analog, etc)
	PointClass mClass;	// class of the point (PC_CLASS<0-3>)
	size_t mIndex;		// index of the measurement

	typedef T MeasType;
};

/**
 * Structure for holding static data information. Adds a deadband and a last
 * event value to the base class.
 */
template<typename T>
struct DLL_LOCAL PointInfo : public PointInfoBase<T> {
	PointInfo(const T& arVal, PointClass aClass, size_t aIndex) :
		PointInfoBase<T>(arVal, aClass, aIndex),
		mDeadband(0),
		mLastEventValue(0),
		mSequence(0)
	{}

	PointInfo() :
		mDeadband(0),
		mLastEventValue(0),
		mSequence(0)
	{}

	double mDeadband;						// deadband associated with measurement (optional)
	typename T::ValueType mLastEventValue;	// the last value that was reported
	size_t mSequence;
};

template <class DataInfoType>
struct DLL_LOCAL StaticIter {
	typedef typename std::vector< DataInfoType >::const_iterator Type;
};

typedef PointInfo<Binary>				BinaryInfo;
typedef PointInfo<Analog>				AnalogInfo;
typedef PointInfo<Counter>				CounterInfo;
typedef PointInfo<ControlStatus>			ControlStatusInfo;
typedef PointInfo<SetpointStatus>			SetpointStatusInfo;
typedef PointInfoBase<VtoData>				VtoDataInfo;

typedef StaticIter<BinaryInfo>::Type			BinaryIterator;
typedef StaticIter<AnalogInfo>::Type			AnalogIterator;
typedef StaticIter<CounterInfo>::Type			CounterIterator;
typedef StaticIter<ControlStatusInfo>::Type		ControlIterator;
typedef StaticIter<SetpointStatusInfo>::Type		SetpointIterator;

} //end namespace

/* vim: set ts=4 sw=4: */

#endif

