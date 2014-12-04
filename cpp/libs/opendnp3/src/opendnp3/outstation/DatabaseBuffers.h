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
#include "opendnp3/outstation/SelectedRanges.h"

#include "opendnp3/outstation/IStaticLoader.h"
#include "opendnp3/outstation/IStaticSelector.h"
#include "opendnp3/outstation/IClassAssigner.h"
#include "opendnp3/outstation/StaticLoadFunctions.h"

namespace opendnp3
{

/**
The database coordinates all updates of measurement data
*/
class DatabaseBuffers : public IStaticSelector, public IStaticLoader, public IClassAssigner, private openpal::Uncopyable
{
public:

	DatabaseBuffers(const DatabaseTemplate&);

	// ------- IStaticSelector -------------

	virtual void Unselect() override final;
	virtual IINField SelectAll(GroupVariation gv) override final;
	virtual IINField SelectRange(GroupVariation gv, const Range& range) override final;

	// ------- IStaticLoader -------------
	
	virtual bool Load(HeaderWriter& writer) override final;
	virtual bool HasAnySelection() const override final { return ranges.HasAnySelection(); }

	// ------- IClassAssigner -------------

	virtual Range AssignClassToAll(AssignClassType type, PointClass clazz) override final;
	virtual Range AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range) override final;
	
	// stores the most revent values and event information	
	StaticBuffers buffers;	

private:
	
	SelectedRanges ranges;	

	template <class T>
	bool LoadType(HeaderWriter& writer);

	template <class T>
	void Deselect()
	{
		auto range = ranges.Get<T>();
		if (range.IsValid())
		{
			auto view = buffers.GetArrayView<T>();
			for (uint16_t i = range.start; i <= range.stop; ++i)
			{
				view[i].selection.selected = false;
			}
			ranges.Clear<T>();
		}		
	}


	static Range RangeOf(uint16_t size);

	template <class T>
	IINField GenericSelect(
		Range range,
		openpal::ArrayView<Cell<T>, uint16_t> view,		
		bool useDefault,
		typename T::StaticVariation variation
	);

	template <class T>
	Range AssignClassTo(PointClass clazz, const Range& range);

	template <class T>
	IINField SelectAll()
	{
		auto view = buffers.GetArrayView<T>();		
		return GenericSelect(RangeOf(view.Size()), view, true, typename T::StaticVariation());
	}

	template <class T>
	IINField SelectAllUsing(typename T::StaticVariation variation)
	{
		auto view = buffers.GetArrayView<T>();
		return GenericSelect(RangeOf(view.Size()), view, false, variation);
	}

	template <class T>
	IINField SelectRange(const Range& range)
	{		
		return GenericSelect(range, buffers.GetArrayView<T>(), true, typename T::StaticVariation());
	}

	template <class T>
	IINField SelectRangeUsing(const Range& range, typename T::StaticVariation variation)
	{		
		return GenericSelect(range, buffers.GetArrayView<T>(), false, variation);
	}		
};

template <class T>
IINField DatabaseBuffers::GenericSelect(
	Range range,
	openpal::ArrayView<Cell<T>, uint16_t> view,	
	bool useDefault,
	typename T::StaticVariation variation)
{
	if (range.IsValid())
	{		
		auto allowed = range.Intersection(RangeOf(view.Size()));

		if (allowed.IsValid())
		{
			// return code depends on if the range was truncated to match the database
			IINField ret = allowed.Equals(range) ? IINField() : IINBit::PARAM_ERROR;

			for (uint16_t i = allowed.start; i <= allowed.stop; ++i)
			{
				if (view[i].selection.selected)
				{
					ret |= IINBit::PARAM_ERROR;
				}
				else
				{
					view[i].selection.selected = true;
					view[i].selection.value = view[i].value;
					view[i].selection.variation = useDefault ? view[i].variation : variation;
				}				
			}

			ranges.Merge<T>(allowed);

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

template <class T>
bool DatabaseBuffers::LoadType(HeaderWriter& writer)
{
	auto range = ranges.Get<T>();
	if (range.IsValid())
	{
		auto view = buffers.GetArrayView<T>();

		bool spaceRemaining = true;

		// ... load values, manipulate the range
		while (spaceRemaining && range.IsValid())
		{
			if (view[range.start].selection.selected)
			{
				/// lookup the specific write function based on the reporting variation
				auto writeFun = GetStaticWriter(view[range.start].selection.variation);

				// start writing a header, the invoked function will advance the range appropriately				
				spaceRemaining = writeFun(view, writer, range);
			}
			else
			{
				// just skip over values that are not selected
				range.Advance();
			}			
		} 		

		ranges.Set<T>(range);

		return spaceRemaining;
	}
	else
	{
		// no data to load
		return true;
	}	
}

template <class T>
Range DatabaseBuffers::AssignClassTo(PointClass clazz, const Range& range)
{
	auto view = buffers.GetArrayView<T>();	
	auto clipped = range.Intersection(RangeOf(view.Size()));
	for (auto i = clipped.start; i <= clipped.stop; ++i)
	{
		view[i].metadata.clazz = clazz;
	}
	return clipped;
}

}

#endif

