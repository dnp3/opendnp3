/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_DATABASEMAPS_H
#define OPENDNP3_DATABASEMAPS_H

#include "app/Range.h"
#include "outstation/IClassAssigner.h"
#include "outstation/IResponseLoader.h"
#include "outstation/IStaticSelector.h"
#include "outstation/IndexSearch.h"
#include "outstation/SelectedRanges.h"
#include "outstation/StaticDataMaps.h"
#include "outstation/StaticWriters.h"

#include "opendnp3/gen/IndexMode.h"
#include "opendnp3/outstation/DatabaseConfigNew.h"
#include "opendnp3/outstation/StaticTypeBitfield.h"

namespace opendnp3
{

/**
The database coordinates all updates of measurement data
*/
class DatabaseMaps final : public IStaticSelector, public IResponseLoader, public IClassAssigner, private Uncopyable
{
public:
    DatabaseMaps(const DatabaseConfigNew& config, StaticTypeBitField allowed_class_zero_types);

    // ------- IStaticSelector -------------

    IINField SelectAll(GroupVariation gv) override;
    IINField SelectRange(GroupVariation gv, const Range& range) override;

    // ------- IResponseLoader -------------

    bool Load(HeaderWriter& writer) override;
    bool HasAnySelection() const override
    {
        return maps.has_any_selection();
    }

    // ------- IClassAssigner -------------

    Range AssignClassToAll(AssignClassType type, PointClass clazz) override;
    Range AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range) override;

    // used to unselect selected points
    void Unselect() override;   

private:

	StaticDataMaps maps;
    StaticTypeBitField allowed_class_zero_types;    

    template<class Spec> bool LoadType(HeaderWriter& writer);

    template<class Spec> void Deselect()
    {
        /* TODO
        auto range = ranges.Get<Spec>();
        if (range.IsValid())
        {
            auto view = buffers.GetArrayView<Spec>();
            for (uint16_t i = range.start; i <= range.stop; ++i)
            {
                view[i].selection.selected = false;
            }
            ranges.Clear<Spec>();
        }
        */
    }

    // specialization for binary in cpp file
    template<class Spec>
    static typename Spec::static_variation_t CheckForPromotion(const typename Spec::meas_t& value,
                                                               typename Spec::static_variation_t variation)
    {
        return variation;
    }

    static Range RangeOf(uint16_t size);

    template<class T>
    IINField GenericSelect(Range range,
                           ser4cpp::ArrayView<Cell<T>, uint16_t> view,
                           bool useDefault,
                           typename T::static_variation_t variation);

    template<class Spec> Range AssignClassTo(PointClass clazz, const Range& range);

    template<class T> void SelectAllClass0()
    {
        if (allowed_class_zero_types.IsSet(T::StaticTypeEnum))
        {
            this->SelectAll<T>();
        }
    }

    template<class T> IINField SelectAll()
    {
        return IINField::Empty();
        /* TODO
        auto view = buffers.GetArrayView<T>();
        return GenericSelect(RangeOf(view.length()), view, true, typename T::static_variation_t());
        */
    }

    template<class T> IINField SelectAllUsing(typename T::static_variation_t variation)
    {
        return IINField::Empty();
        /* TODO
        auto view = buffers.GetArrayView<T>();
        return GenericSelect(RangeOf(view.length()), view, false, variation);
        */
    }

    template<class T>
    IINField SelectVirtualRange(const Range& range, bool usedefault, typename T::static_variation_t variation)
    {
       return IINField::Empty();

	   /*
       return GenericSelect(range, buffers.GetArrayView<T>(), usedefault, variation);
       */

    }

    template<class T> IINField SelectRange(const Range& range)
    {
        return SelectVirtualRange<T>(range, true, typename T::static_variation_t());
    }

    template<class T> IINField SelectRangeUsing(const Range& range, typename T::static_variation_t variation)
    {
        return SelectVirtualRange<T>(range, false, variation);
    }
};

template<class T>
IINField DatabaseMaps::GenericSelect(Range range,
                                        ser4cpp::ArrayView<Cell<T>, uint16_t> view,
                                        bool useDefault,
                                        typename T::static_variation_t variation)
{
    return IINField();
    /* TODO
    if (range.IsValid())
    {
        auto allowed = range.Intersection(RangeOf(view.length()));

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
                    auto var = useDefault ? view[i].config.svariation : variation;
                    view[i].selection.variation = CheckForPromotion<T>(view[i].selection.value, var);
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
    */
}

template<class T> bool DatabaseMaps::LoadType(HeaderWriter& writer)
{
	// TODO
    return false;

    /*
    auto range = ranges.Get<T>();
    if (!range.IsValid())
        return true; // no data to load

    auto view = buffers.GetArrayView<T>();

    bool spaceRemaining = true;

    // ... load values, manipulate the range
    while (spaceRemaining && range.IsValid())
    {
        if (view[range.start].selection.selected)
        {
            /// lookup the specific write function based on the reporting variation
            auto writeFun = StaticWriters::Get(view[range.start].selection.variation);

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
    */
}

template<class Spec> Range DatabaseMaps::AssignClassTo(PointClass clazz, const Range& range)
{
    return range;

	/* TODO
    auto view = buffers.GetArrayView<Spec>();
    auto clipped = range.Intersection(RangeOf(view.length()));
    for (auto i = clipped.start; i <= clipped.stop; ++i)
    {
        view[i].config.clazz = clazz;
    }
    return clipped;
    */
}

} // namespace opendnp3

#endif
