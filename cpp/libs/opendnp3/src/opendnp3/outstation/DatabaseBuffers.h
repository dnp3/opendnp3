/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_DATABASEBUFFERS_H
#define OPENDNP3_DATABASEBUFFERS_H

#include "opendnp3/app/Range.h"

#include "opendnp3/outstation/DatabaseTemplate.h"
#include "opendnp3/outstation/StaticBuffers.h"
#include "opendnp3/outstation/StaticSelection.h"
#include "opendnp3/outstation/IStaticSelector.h"
#include "opendnp3/outstation/SelectedRanges.h"

namespace opendnp3
{

/**
The database coordinates all updates of measurement data
*/
class DatabaseBuffers : public IStaticSelector, private openpal::Uncopyable
{
public:

	DatabaseBuffers(const DatabaseTemplate&);

	// ------- IStaticSelector -------------

	virtual void DeselectAll() override final;
	virtual IINField SelectAll(GroupVariation gv) override final;
	virtual IINField SelectRange(GroupVariation gv, const Range& range) override final;

	// stores the most revent values and event information
	StaticBuffers current;

private:

	StaticSelection selected;
	SelectedRanges ranges;

	template <class T>
	void Deselect()
	{
		auto& range = ranges.GetRange<T>();
		if (range.IsValid())
		{
			auto view = selected.GetArrayView<T>();
			for (uint16_t i = range.start; i <= range.stop; ++i)
			{
				view[i].selected = false;
			}
		}
		range = Range::Invalid();
	}


	static Range RangeOf(const openpal::HasSize<uint16_t>& sized);

	template <class T>
	IINField GenericSelect(
		Range range,
		openpal::ArrayView<MeasurementCell<T>, uint16_t> current,
		openpal::ArrayView<BufferedCell<T>, uint16_t> frozen,	
		bool useDefault,
		typename T::StaticVariation variation
	);

	template <class T>
	IINField SelectAll()
	{
		auto view = current.GetArrayView<T>();		
		return GenericSelect(RangeOf(view), view, selected.GetArrayView<T>(), true, typename T::StaticVariation());
	}

	template <class T>
	IINField SelectAllUsing(typename T::StaticVariation variation)
	{
		auto view = current.GetArrayView<T>();
		return GenericSelect(RangeOf(view), view, selected.GetArrayView<T>(), false, variation);
	}

	template <class T>
	IINField SelectRange(const Range& range)
	{		
		return GenericSelect(range, current.GetArrayView<T>(), selected.GetArrayView<T>(), true, typename T::StaticVariation());
	}

	template <class T>
	IINField SelectRangeUsing(const Range& range, typename T::StaticVariation variation)
	{		
		return GenericSelect(range, current.GetArrayView<T>(), selected.GetArrayView<T>(), false, variation);
	}		
};

template <class T>
IINField DatabaseBuffers::GenericSelect(
	Range range,
	openpal::ArrayView<MeasurementCell<T>, uint16_t> current,
	openpal::ArrayView<BufferedCell<T>, uint16_t> selected,
	bool useDefault,
	typename T::StaticVariation variation)
{
	if (range.IsValid())
	{		
		auto allowed = range.Intersection(RangeOf(current));

		if (allowed.IsValid())
		{
			// return code depends on if the range was truncated to match the database
			IINField ret = allowed.Equals(range) ? IINField() : IINBit::PARAM_ERROR;

			for (uint16_t i = allowed.start; i < allowed.stop; ++i)
			{
				if (selected[i].selected)
				{
					ret |= IINBit::PARAM_ERROR;
				}
				else
				{
					selected[i].selected = true;
					selected[i].value = current[i].value;
					selected[i].variation = useDefault ? current[i].variation : variation;
				}				
			}

			ranges.SetRange<T>(allowed);

			return ret;
		}
		else
		{
			return IINField(IINBit::PARAM_ERROR);
		}
	}
	else
	{
		return IINField();
	}
}

}

#endif

