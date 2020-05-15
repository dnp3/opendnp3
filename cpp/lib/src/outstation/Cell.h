/*
 * Copyright 2013-2020 Automatak, LLC
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
#ifndef OPENDNP3_STATICDATACELL_H
#define OPENDNP3_STATICDATACELL_H

namespace opendnp3
{

/**
 * Type used to record whether a value is requested in a response
 */
template<class Spec> struct SelectedValue
{
    SelectedValue() = default;

    SelectedValue(bool selected, const typename Spec::meas_t& value, typename Spec::static_variation_t variation)
        : selected(selected), value(value), variation(variation)
    {
    }

    bool selected = false;

    typename Spec::meas_t value;
    typename Spec::static_variation_t variation = Spec::DefaultStaticVariation;
};

/**
 * Holds particular measurement type in the database.
 */
template<class Spec> struct StaticDataCell
{
    typename Spec::meas_t value;       // current value
    typename Spec::config_t config;    // configuration
    typename Spec::event_cell_t event; // event cell
    SelectedValue<Spec> selection;     // selected value

	StaticDataCell() = default;
    StaticDataCell(const typename Spec::meas_t& value, const typename Spec::config_t& config)
        : value(value), config(config) {}
    StaticDataCell(const typename Spec::config_t& config) : config(config) {}
};

} // namespace opendnp3

#endif
